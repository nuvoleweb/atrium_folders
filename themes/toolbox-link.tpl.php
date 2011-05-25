<?php
/**
 * @file
 * Toolbox link template file.
 */
?>
<div class="modal-tooltip-container">
  <?php print l($text, $dest, array('html' => TRUE, 'attributes' => array('id' => $dom_id, 'class' => "$class no-ctools-use-ajax modal-toolbox modal-tooltip-link", 'title' => $alt))); ?>
  <div id="<?php print $dom_id ?>-block" class="modal-tooltip-block"></div>
</div>



