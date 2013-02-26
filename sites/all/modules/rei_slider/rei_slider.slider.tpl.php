<?php
$img_count = variable_get('no_of_image', 5);
?>
    <section id="slider">
    <ul class="slides">
    	<?php for($i=1; $i<=$img_count; $i++): ?>
      <li>
        <article class="post">
        <div class="entry-container">
          <header class="entry-header">
            <h2 class="entry-title"><a href="#"><?php print variable_get('slider_title_' . $i, 'Sample product'); ?></a></h2>
          </header><!-- .entry-header -->
          <div class="entry-summary">
          <?php print variable_get('slider_desc_' . $i, 'Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt... '); ?>
          </div><!-- .entry-summary -->
          <div class="clear"></div>
        </div><!-- .entry-container -->
            <a href="#">
     <?php       
	$file = file_load(variable_get('slider_image_' . $i, ''));
  $variables = array(
      'path' => file_create_url($file->uri), 
      'alt' => 'Test alt',
      'title' => 'Test title',
      'width' => '50%',
      'height' => '25%',
      'attributes' => array('class' => 'slide-image'),
      );
   print theme('image', $variables);
   ?>
            </a>
         <div class="clear"></div>
        </article>
      </li>
      <?php endfor; ?>      
    </ul>
    </section>