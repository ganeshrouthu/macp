jQuery(document).ready(function(){
   jQuery(".tog").hide();
   jQuery(".clk2").css('display','none');
  jQuery(".clk1").click(function(){
    jQuery(".tog").toggle();
    jQuery(".clk1").css('display','none');
    jQuery(".clk2").css('display','inline');
  });
   jQuery(".clk2").click(function(){
    jQuery(".tog").toggle();
    jQuery(".clk2").css('display','none');
    jQuery(".clk1").css('display','inline');
  });
   jQuery("#mainz").css('display','none');
  jQuery("#readmr").click(function(){
    jQuery("#teazer").css('display','none');
    jQuery("#mainz").css('display','block');
  });
  //var minheight = Math.max(jQuery(".region-sidebar-first").height(),jQuery(".region-sidebar-second").height());
  var minheight = jQuery(".region-sidebar-first").height();
  
  //var hgt = minheight - jQuery("h1#page-title").height()-Math.max(jQuery(".breadcrumb").height())-50;
  var hgt = minheight - jQuery("h1#page-title").height()-60;
  if(jQuery("#teazer-body").height()<hgt){
  jQuery("#gotop").hide();
  }
  jQuery("#teazer-body").css('max-height',hgt);
  jQuery("#gotop").click(function(){
    jQuery("#teazer-body").css('max-height','none');
    jQuery("#mainz").css('display','none');
    jQuery("#teazer").css('display','block');
    jQuery("#gotop").hide();
  });
	// jquery for Activity, Case Study, Innovations and Success Story slide shows.
	jQuery("a.colorbox").colorbox({rel:'my-gallery'});
});