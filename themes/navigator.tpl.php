<?php
/**
 * @file
 * Folder navigator template file.
 */
?>
<div class="atrium-folders-navigator">
  <div class="atrium-folders-navigator-inner clear-block">
    <div class="toolbar clear-block">
        <?php print $form; ?>
    </div>
    <div class="navigator clear-block">
      <div class="left">
        <?php print views_embed_view('folders_navigation', 'default', $node->nid); ?>
      </div>
      <div id="attach-wrapper" class="right">
        <?php print views_embed_view('folders_folders', 'block_1', $node->nid); ?>
        <?php print views_embed_view('folders_files', 'default', $node->nid); ?>
      </div>
    </div>
  </div>
</div>



