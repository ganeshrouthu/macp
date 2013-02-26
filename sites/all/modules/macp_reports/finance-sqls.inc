<?php
function _sql_ddrcs_yearly_report($type, $financial_year, $district_id = 'all') {
  $sql_district_id = ($district_id == '' || $district_id == 'all') ? '' : " District_ID = " . $district_id ." AND ";
  $sql_fin_year        =  "SELECT [Financial_Year_ID]
                            FROM [MACPDB].[dbo].[T_Financial_Year]
                              WHERE Financial_Year_Name = '" . $financial_year . "'";
  $sql_fin_flag = "";
  if ($financial_year != 'all' && $financial_year != '') {
    $sql_fin_flag = "AND Financial_Year_ID = ($sql_fin_year)";
  }  
  $sql_id = "SELECT [Accounting_Centers_ID] FROM [MACPDB].[dbo].[T_Accounting_Centers]
                            WHERE Accounting_Centers_ID = E.Accounting_Centers_ID";
  $sql_fina = "(SELECT DISTINCT(SELECT [Financial_Year_Name] 
  FROM [MACPDB].[dbo].[T_Financial_Year]
  WHERE Financial_Year_ID = TFA.Financial_Year_ID
  ) FROM [MACPDB].[dbo].[T_FundAllocation] AS TFA
                          WHERE Accounting_Centers_ID = E.Accounting_Centers_ID
                          AND Financial_Year_ID = E.Financial_Year_ID) AS Financial_Year,";
  $sql_acc_center_name = "SELECT [Accounting_Centers_Name] FROM [MACPDB].[dbo].[T_Accounting_Centers]
                            WHERE Accounting_Centers_ID = E.Accounting_Centers_ID ";
  $sql_grand_total     =  "SELECT SUM(GrandTotalAmt) FROM [MACPDB].[dbo].[T_Expenditure] 
                            WHERE Accounting_Centers_ID = E.Accounting_Centers_ID 
                            AND Financial_Year_ID = E.Financial_Year_ID";
  
  $sql_grand_total_one = "SELECT SUM([GrandTotalAmt])
                            FROM [MACPDB].[dbo].[T_FundAllocation]
                            WHERE Accounting_Centers_ID = E.Accounting_Centers_ID
                            AND Financial_Year_ID = E.Financial_Year_Id";
  $sql_acc_center_id  = "SELECT [Accounting_Centers_ID]
                          FROM [MACPDB].[dbo].[T_Accounting_Centers]
                          WHERE 
                          $sql_district_id
                          Accounting_Centers_Type_ID = (SELECT [Account_Center_Type_ID]
                          FROM [MACPDB].[dbo].[T_Account_Center_Type]
                          WHERE Account_Center_Type_Value = '" . $type . "')";
  $sql_ben_total_amt = "SELECT SUM([BenTotAmt]) FROM [MACPDB].[dbo].[T_FundAllocation]
                          WHERE Accounting_Centers_ID = E.Accounting_Centers_ID
                          AND Financial_Year_ID = E.Financial_Year_Id"; 
  $sql_ben_total_amt_two = "SELECT SUM([BenTotAmt]) FROM [MACPDB].[dbo].[T_Expenditure] 
                              WHERE 
                              
                              Accounting_Centers_ID = E.Accounting_Centers_ID 
                              AND Financial_Year_ID = E.Financial_Year_ID";
  $sql_district_id = "SELECT District_ID
  FROM [MACPDB].[dbo].[T_Accounting_Centers]
  WHERE 
  $sql_district_id
  Accounting_Centers_ID = E.Accounting_Centers_ID";
 $sql = "
 SELECT 
  DISTINCT($sql_acc_center_name) AS DDRCS_Name, 
  
  ($sql_id) AS ACC_ID, 
  
  
  
  (SELECT [Dist_PhaseNo] FROM [MACPDB].[dbo].[T_District]
    WHERE Dist_ID = ($sql_district_id)) AS Phase,
    $sql_fina
  ($sql_grand_total_one) AS Project_Funds_Released,
  ($sql_grand_total) AS Project_Funds_Expenditure,
  ($sql_ben_total_amt) AS Beneficiary_Funds_Collection,
  ($sql_ben_total_amt_two) AS Beneficiary_Funds_Expenditure  
    FROM [MACPDB].[dbo].[T_Expenditure] AS E
    WHERE Accounting_Centers_ID IN ($sql_acc_center_id)
$sql_fin_flag
AND ($sql_grand_total) IS NOT NULL
AND ($sql_grand_total_one) IS NOT NULL
AND ($sql_ben_total_amt) IS NOT NULL
AND ($sql_ben_total_amt_two) IS NOT NULL
";
  return $sql;
}

function _sql_atma_yearly_report($type, $financial_year, $district_id = NULL) {
  $sql_district_id = ($district_id == '' || $district_id == 'all') ? '' : " District_ID = " . $district_id ." AND ";
  //$financial_year = ($financial_year == 'all') ? '' : ''; 
  if (! $financial_year || $financial_year == 'all') {
    $financial_year = (date('Y')-1) . '-' . date('Y');
  }
  
  $sql_finance = " AND Financial_Year_ID >= (SELECT [Financial_Year_ID]
      FROM [MACPDB].[dbo].[T_Financial_Year]
      WHERE Financial_Year_Name = '" . $financial_year . "')";
  $last_financial_year = _get_last_financial_year($financial_year);
  
  $sql_atma = "SELECT DISTINCT([Accounting_Centers_Name])
      
      ,(SELECT [Dist_PhaseNo] 
  FROM [MACPDB].[dbo].[T_District]
  WHERE Dist_ID = AC.District_ID) AS phase
,
(SELECT SUM([GrandTotalAmt])
FROM [MACPDB].[dbo].[T_FundAllocation]
WHERE Accounting_Centers_ID = AC.Accounting_Centers_ID
) AS Funds_Released,

(SELECT SUM([GrandTotalAmt])
  FROM [MACPDB].[dbo].[T_Expenditure]
  WHERE Accounting_Centers_ID = AC.Accounting_Centers_ID
  $sql_finance) AS Expenditure_Year_Till_Date ,

(SELECT SUM([GrandTotalAmt]) AS Expenditure
  FROM [MACPDB].[dbo].[T_Expenditure]
  WHERE Accounting_Centers_ID = AC.Accounting_Centers_ID) AS Expenditure,

  (SELECT SUM([GrandTotalAmt]) AS Expenditure_Year_Till_Date
  FROM [MACPDB].[dbo].[T_Expenditure]
  WHERE Accounting_Centers_ID = AC.Accounting_Centers_ID
  AND Financial_Year_ID = (SELECT [Financial_Year_ID]
  FROM [MACPDB].[dbo].[T_Financial_Year]
  WHERE Financial_Year_Name = '" . $last_financial_year . "')) AS Last_Financial_Year_Expenditure 

  FROM [MACPDB].[dbo].[T_Accounting_Centers] AS AC
    WHERE
  " . $sql_district_id . 
  "  Accounting_Centers_Type_ID = (SELECT Account_Center_Type_ID FROM [MACPDB].[dbo].[T_Account_Center_Type]
WHERE Account_Center_Type_Value = '" . $type . "')
AND 
(SELECT SUM([GrandTotalAmt])
FROM [MACPDB].[dbo].[T_FundAllocation]
WHERE Accounting_Centers_ID = AC.Accounting_Centers_ID
) IS NOT NULL
ORDER BY Accounting_Centers_Name
";
  return $sql_atma;
}

function _sql_dyAHD_yearly_report($type, $financial_year, $district_id = 'all') {
  $last_financial_year = _get_last_financial_year($financial_year);
  $sql_district_id = ($district_id == 'all') ? '' : " AND District_ID = " . $district_id;
//HAVING SUM(GrandTotalAmt) != NULL

  $sql = "SELECT DISTINCT(AC.[Accounting_Centers_Name]) AS Accounting_Center,
  (SELECT [Dist_PhaseNo]
    FROM [MACPDB].[dbo].[T_District]
    WHERE Dist_ID IN (
      SELECT [District_ID] 
      FROM [MACPDB].[dbo].[T_Accounting_Centers]
      WHERE Accounting_Centers_ID = AC.Accounting_Centers_ID
    )) AS Phase
    
    ,(SELECT SUM([GrandTotalAmt])
  FROM [MACPDB].[dbo].[T_FundAllocation]
  WHERE 
  Accounting_Centers_ID = AC.Accounting_Centers_ID) AS Funds_Released
    ,
(SELECT SUM(GrandTotalAmt)
  FROM [MACPDB].[dbo].[T_Expenditure]
  WHERE 
  Financial_Year_ID = (SELECT [Financial_Year_ID]      
  FROM [MACPDB].[dbo].[T_Financial_Year]
  WHERE Financial_Year_Name = '" . $last_financial_year . "')
  AND
  Accounting_Centers_ID = AC.Accounting_Centers_ID) AS Expdit_By_Last_Fin_Year,
(SELECT SUM(GrandTotalAmt)
    FROM [MACPDB].[dbo].[T_Expenditure]
    WHERE 
    Financial_Year_ID >= (SELECT [Financial_Year_ID]      
    FROM [MACPDB].[dbo].[T_Financial_Year]
    WHERE Financial_Year_Name = '" . $financial_year . "'
    )
    AND
      Accounting_Centers_ID = AC.Accounting_Centers_ID) AS Expdit_Year_Till_Date,
(SELECT SUM(GrandTotalAmt)
  FROM [MACPDB].[dbo].[T_Expenditure]
  WHERE 
  Accounting_Centers_ID = AC.Accounting_Centers_ID) AS Expenditure
  FROM [MACPDB].[dbo].[T_Accounting_Centers] AS AC
  WHERE AC.Accounting_Centers_Type_ID = (SELECT [Account_Center_Type_ID] 
  FROM [MACPDB].[dbo].[T_Account_Center_Type]
  WHERE Account_Center_Type_Value = 'Dy. AHD')
  $sql_district_id
  ";
  return $sql;
}

function _sql_complaints_deptwise_yearly_report($financial_year = 'all') {
  $district_id = (arg(3) == 'all' || !arg(3)) ? FALSE : arg(3);
  //$sql_district_id = ($district_id == 'all') ? '' : " AND Dist_ID = " . $district_id;
  $sql_district_id = ($district_id == '' || $district_id == 'all') ? '' : "AND Dist_ID = " . $district_id;
  $year_from = substr($financial_year, 0, 4);
  $year_to = substr($financial_year, 5, 4);
  $sql_year = '';
  if ($financial_year != 'all' && $financial_year) {
    $sql_year = "WHERE App_Submission_Date BETWEEN '" . $year_from . "-01-01 01:01:01.000'
      AND '".$year_to."-01-01 01:01:01.000'";
  }
  else {
   $sql_year = "WHERE App_Submission_Date < '" . date('Y') . "-01-01 01:01:01.000'";
  }
  $sql  = "SELECT 
(SELECT [DepartmentName]
		FROM [MACPDB].[dbo].[T_Department]
		WHERE DepartmentID = CA.DepartmentID) AS Department_Name

/** Total Complaints start **/
,
COUNT([Complaint_ID]) AS Total_Complaints
/** Total complaints end **/
,
/** New complaints start **/
(	SELECT COUNT(CmplntBucket_ID) FROM T_Complaint_Work_Bucket
		WHERE Complaint_ID 
			IN (SELECT Complaint_ID FROM T_Cmplnt_Application
					WHERE DepartmentID = CA.[DepartmentID])
					AND Status_Id = (SELECT [Status_Id]
				FROM [MACPDB].[dbo].[T_Status]
			WHERE Status_Value = 'New Complaint')) AS Count_New_Complaints 
/** New complaints end **/
,
/** New complaints start **/
(	SELECT COUNT(CmplntBucket_ID) FROM T_Complaint_Work_Bucket
		WHERE Complaint_ID 
			IN (SELECT Complaint_ID FROM T_Cmplnt_Application
					WHERE DepartmentID = CA.[DepartmentID])
					AND Status_Id = (SELECT [Status_Id]
				FROM [MACPDB].[dbo].[T_Status]
			WHERE Status_Value = 'Interim Reply Sent')) AS Count_Interim_Reply_Sent 
/** New complaints end **/
,
/** Forwarded complaints start **/
(	SELECT COUNT(CmplntBucket_ID) FROM T_Complaint_Work_Bucket
		WHERE Complaint_ID 
			IN (SELECT Complaint_ID FROM T_Cmplnt_Application
					WHERE DepartmentID = CA.[DepartmentID])
					AND Status_Id = (SELECT [Status_Id]
				FROM [MACPDB].[dbo].[T_Status]
			WHERE Status_Value = 'Forwarded')) AS Count_Forwarded 
/** Forwarded complaints end **/
,
/** Forwarded complaints start **/
(	SELECT COUNT(CmplntBucket_ID) FROM T_Complaint_Work_Bucket
		WHERE Complaint_ID 
			IN (SELECT Complaint_ID FROM T_Cmplnt_Application
					WHERE DepartmentID = CA.[DepartmentID])
					AND Status_Id = (SELECT [Status_Id]
				FROM [MACPDB].[dbo].[T_Status]
			WHERE Status_Value = 'Resolved')) AS Count_Resolved 
/** Forwarded complaints end **/
  FROM [MACPDB].[dbo].[T_Cmplnt_Application] AS CA
  $sql_year
  $sql_district_id
  GROUP BY CA.DepartmentID";
  return $sql;
}

function _sql_complaints_dist_phasewise_yearly_report($financial_year) {
  $year_from = substr($financial_year, 0, 4);
  $year_to = substr($financial_year, 5, 4);
  $district_id = (arg(3) == 'all' || !arg(3)) ? '' : arg(3);
//  $sql_district_id = ($district_id == 'all') ? '' : " AND Dist_ID = " . $district_id;
  
  $sql_district_id = ($district_id == '' || $district_id == 'all') ? '' : " AND Dist_ID = " . $district_id;
  
  $sql_year = '';
  if ($financial_year != 'all' && $financial_year) {
    $sql_year = " CA.App_Submission_Date BETWEEN '" . $year_from . "-01-01 01:01:01.000'
  AND '".$year_to."-01-01 01:01:01.000'";  
  }
  else {
   $sql_year = " App_Submission_Date < '" . date('Y') . "-01-01 01:01:01.000'";
  }

  $sql = "SELECT 
		(SELECT Dist_PhaseNo FROM [MACPDB].[dbo].[T_District] WHERE Dist_ID = CA.Dist_ID) AS Dist_Phase
		,
		(SELECT [Dist_Name] FROM [MACPDB].[dbo].[T_District] WHERE Dist_ID = CA.Dist_ID) AS District
		,
		COUNT(CA.[Complaint_ID]) AS Total_Complaints
		,
		/** New complaints start **/
(SELECT COUNT(CmplntBucket_ID) FROM T_Complaint_Work_Bucket
		WHERE Complaint_ID 
			IN (SELECT Complaint_ID FROM T_Cmplnt_Application
					WHERE Dist_ID = CA.[Dist_ID])
					AND Status_Id = (SELECT [Status_Id]
				FROM [MACPDB].[dbo].[T_Status]
			WHERE Status_Value = 'New Complaint')) AS Count_New_Complaints 
/** New complaints end **/
,
		/** Interim Reply Sent complaints start **/
(SELECT COUNT(CmplntBucket_ID) FROM T_Complaint_Work_Bucket
		WHERE Complaint_ID 
			IN (SELECT Complaint_ID FROM T_Cmplnt_Application
					WHERE Dist_ID = CA.[Dist_ID])
					AND Status_Id = (SELECT [Status_Id]
				FROM [MACPDB].[dbo].[T_Status]
			WHERE Status_Value = 'Interim Reply Sent')) AS Count_Interim_Reply_Sent 
/** Interim Reply Sent complaints end **/
,
		/** Forwarded complaints start **/
(SELECT COUNT(CmplntBucket_ID) FROM T_Complaint_Work_Bucket
		WHERE Complaint_ID 
			IN (SELECT Complaint_ID FROM T_Cmplnt_Application
					WHERE Dist_ID = CA.[Dist_ID])
					AND Status_Id = (SELECT [Status_Id]
				FROM [MACPDB].[dbo].[T_Status]
			WHERE Status_Value = 'Forwarded')) AS Count_Forwarded 
/** Forwarded complaints end **/
,
		/** Resolved complaints start **/
(SELECT COUNT(CmplntBucket_ID) FROM T_Complaint_Work_Bucket
		WHERE Complaint_ID 
			IN (SELECT Complaint_ID FROM T_Cmplnt_Application
					WHERE Dist_ID = CA.[Dist_ID])
					AND Status_Id = (SELECT [Status_Id]
				FROM [MACPDB].[dbo].[T_Status]
			WHERE Status_Value = 'Resolved')) AS Count_Resolved 
/** Resolved complaints end **/
		
  FROM [MACPDB].[dbo].[T_Cmplnt_Application] AS CA
  WHERE $sql_year 
  $sql_district_id
  GROUP BY CA.Dist_ID";
  return $sql;
}

function _sql_rh_cwp($financial_year) {
  
  $year_from = substr($financial_year, 0, 4);
  
  $district_id = (arg(3) == 'all' || !arg(3)) ? FALSE : arg(3);
  $sql_district_id = ($district_id == 'all') ? '' : " AND Dist_ID = " . $district_id;

  $sql_year = '';
  if ($financial_year != 'all' && $financial_year) {
    $sql_year = "Year_ID <= (
												SELECT [Financial_Year_ID] FROM [MACPDB].[dbo].[T_Financial_Year] 
												WHERE Financial_Year_Name LIKE '".$year_from."-%')";
  }
  else {
   $sql_year = "Year_ID <= (
												SELECT [Financial_Year_ID] FROM [MACPDB].[dbo].[T_Financial_Year] 
												WHERE Financial_Year_Name LIKE '".date('Y')."-%')";
  }

  $sql  = "SELECT
	(SELECT [CWP_FPP_Stage_Value] FROM [MACPDB].[dbo].[T_ME_CWP_FPP_Stage] WHERE [CWP_FPP_Stage_ID] = CW.CWP_FPP_Stage_ID) AS Work_Progress_Stage,
	(SELECT COUNT([RH_ID]) FROM T_ME_RH_CWP_HIS WHERE InPerCivilWork = '25.00' AND CWP_FPP_Stage_ID = CW.CWP_FPP_Stage_ID) AS Per_25,
	(SELECT COUNT([RH_ID]) FROM T_ME_RH_CWP_HIS WHERE InPerCivilWork = '50.00' AND CWP_FPP_Stage_ID = CW.CWP_FPP_Stage_ID) AS Per_50,
	(SELECT COUNT([RH_ID]) FROM T_ME_RH_CWP_HIS WHERE InPerCivilWork = '75.00' AND CWP_FPP_Stage_ID = CW.CWP_FPP_Stage_ID) AS Per_75,
	(SELECT COUNT([RH_ID]) FROM T_ME_RH_CWP_HIS WHERE InPerCivilWork = '100.00' AND CWP_FPP_Stage_ID = CW.CWP_FPP_Stage_ID) AS Per_100,
	(SELECT SUM(ProposedCost) FROM [MACPDB].[dbo].[T_ME_RH_CWP_HIS] WHERE CWP_FPP_Stage_ID = CW.CWP_FPP_Stage_ID) AS Total_Proposed_Cost,
	(SELECT SUM(PaymentDone) FROM [MACPDB].[dbo].[T_ME_RH_CWP_HIS] WHERE CWP_FPP_Stage_ID = CW.CWP_FPP_Stage_ID) AS Total_Payment_Done,
	(SELECT AVG(PhysicalVsFinancial) FROM [MACPDB].[dbo].[T_ME_RH_CWP_HIS] WHERE CWP_FPP_Stage_ID = CW.CWP_FPP_Stage_ID) AS Phy

FROM [MACPDB].[dbo].[T_ME_RH_CWP_HIS] AS CW  WHERE $sql_year
		AND 1 IN (
					SELECT [Dist_PhaseNo] FROM [MACPDB].[dbo].[T_District] WHERE Dist_ID IN 
          (
          
          SELECT [Dist_ID] 
  FROM [MACPDB].[dbo].[T_District]
  WHERE Dist_ID IN (
          SELECT [Dist_ID] 
          FROM [MACPDB].[dbo].[T_ME_RH]
          WHERE RH_ID IN (SELECT [RH_ID] FROM T_ME_RH_CWP_HIS WHERE CWP_FPP_Stage_ID = CW.CWP_FPP_Stage_ID)
  )
          )
          GROUP BY Dist_PhaseNo
				)";
    if ($district_id) {
      $sql .= "AND CW.RH_ID IN (SELECT RH_ID FROM [MACPDB].[dbo].[T_ME_RH] WHERE Dist_ID = " . $district_id.")";
    }
    $sql .= " GROUP BY CW.CWP_FPP_Stage_ID";
  return $sql;
}

function _sql_apmc_cwp($financial_year) {
  $district_id = (arg(3) == 'all' || !arg(3)) ? FALSE : arg(3);
  $Dist_string = ($district_id == 'all' || $district_id == '') ? '' : " AND Dist_ID = " . $district_id;
  $year_from = substr($financial_year, 0, 4);

  $sql_per_25 = "(SELECT COUNT([APMC_ID]) FROM T_ME_APMC_CWP_His WHERE InPerCivilWork = '25.00' AND CWP_FPP_Stage_ID = CW.CWP_FPP_Stage_ID) AS Per_25";
  
  $sql_per_50 = "(SELECT COUNT([APMC_ID]) FROM T_ME_APMC_CWP_His WHERE InPerCivilWork = '25.00' AND CWP_FPP_Stage_ID = CW.CWP_FPP_Stage_ID) AS Per_50";

  $sql_per_75 = "(SELECT COUNT([APMC_ID]) FROM T_ME_APMC_CWP_His WHERE InPerCivilWork = '25.00' AND CWP_FPP_Stage_ID = CW.CWP_FPP_Stage_ID) AS Per_75";

  $sql_per_100 = "(SELECT COUNT([APMC_ID]) FROM T_ME_APMC_CWP_His WHERE InPerCivilWork = '25.00' AND CWP_FPP_Stage_ID = CW.CWP_FPP_Stage_ID) AS Per_100";

  $sql_proposed_cost = "(SELECT SUM(ProposedCost) FROM [MACPDB].[dbo].[T_ME_APMC_CWP_His] 
        WHERE CWP_FPP_Stage_ID = CW.CWP_FPP_Stage_ID) AS Total_Proposed_Cost";
  $sql_work_progress_stage = "(SELECT [CWP_FPP_Stage_Value] FROM [MACPDB].[dbo].[T_ME_CWP_FPP_Stage] 
        WHERE [CWP_FPP_Stage_ID] = CW.CWP_FPP_Stage_ID) AS Work_Progress_Stage";
  $sql_total_payment_done = "(SELECT SUM(PaymentDone) FROM [MACPDB].[dbo].[T_ME_APMC_CWP_His] 
        WHERE CWP_FPP_Stage_ID = CW.CWP_FPP_Stage_ID) AS Total_Payment_Done";
  $sql_phy = "(SELECT AVG(PhysicalVsFinancial) FROM [MACPDB].[dbo].[T_ME_APMC_CWP_His] 
        WHERE CWP_FPP_Stage_ID = CW.CWP_FPP_Stage_ID) AS Phy";
  $sql_apmc_id_in = "SELECT DISTINCT(APMC_ID) FROM T_ME_APMC WHERE District_ID IN (
      SELECT DISTINCT(Dist_ID) FROM T_District
      WHERE Dist_PhaseNo = 1 " . $Dist_string . ")";
      
  $sql_year = '';
  if ($financial_year != 'all' && $financial_year != '') {
    $sql_year = "AND Year_ID <= (SELECT [Financial_Year_ID] FROM [MACPDB].[dbo].[T_Financial_Year] 
    WHERE Financial_Year_Name LIKE '".$year_from."-%')";
  }

  $sql  = "
    SELECT
      $sql_work_progress_stage,
      $sql_per_25,
      $sql_per_50,
      $sql_per_75,
      $sql_per_100,
      $sql_proposed_cost,
      $sql_total_payment_done,
      $sql_phy

  FROM [MACPDB].[dbo].[T_ME_APMC_CWP_His] AS CW 
  WHERE APMC_ID IN ($sql_apmc_id_in)
  $sql_year
  GROUP BY CW.CWP_FPP_Stage_ID";
  return $sql;
}

function _sql_districts() {
  $sql_districts = "SELECT [Dist_ID]
      ,[Dist_PhaseNo]
      ,[Dist_Name]      
  FROM [MACPDB].[dbo].[T_District]
  ORDER BY Dist_PhaseNo ";
  return $sql_districts;
}

function _sql_districts_rhs($district_id = 'all') {
/* GET District wise RH Data for Phase 1*/
  $sql_district_id = ($district_id == 'all' || $district_id == '') ? '' : " AND Dist_ID = " . $district_id;
  $sql = "SELECT [Dist_Name]
      ,(SELECT COUNT([RH_ID])
  FROM [MACPDB].[dbo].[T_ME_RH]
  WHERE Dist_ID = DST.[Dist_ID]) AS Num_Centers
  FROM [MACPDB].[dbo].[T_District] AS DST
  WHERE Dist_PhaseNo = 1 $sql_district_id
  ORDER BY Dist_PhaseNo";
  return $sql;
}

function _sql_districts_apmcs($district_id = 'all') {
/* GET District wise APMCs Data for Phase 1*/
  $sql_district_id = ($district_id == 'all' || $district_id == '') ? '' : " AND Dist_ID = " . $district_id;
  $sql = "SELECT [Dist_Name]
          ,   (
                SELECT COUNT([APMC_ID])
                  FROM [MACPDB].[dbo].[T_ME_APMC]
                  WHERE District_ID = DST.[Dist_ID]
              ) AS Num_Centers
          FROM [MACPDB].[dbo].[T_District] AS DST
          WHERE Dist_PhaseNo = 1 " . $sql_district_id . " ORDER BY Dist_PhaseNo";
  //_p($sql, 1);
  return $sql;
}

function _sql_financial_years() {
  $sql_districts = "SELECT [Financial_Year_ID]
      ,[Financial_Year_Name]
  FROM [MACPDB].[dbo].[T_Financial_Year]";
  return $sql_districts;
}
