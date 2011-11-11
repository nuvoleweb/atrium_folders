/**
 * @file
 * Implement a modal toolbox.
 */

(function ($) {

	Drupal.AtriumFolders = Drupal.AtriumFolders || {};
	Drupal.AtriumFolders.ModalToolbox = Drupal.AtriumFolders.ModalToolbox || {};
	
	/**
	 * Display the modal
	 */
	Drupal.AtriumFolders.ModalToolbox.show = function(element) {

	  Drupal.AtriumFolders.ModalToolbox.modal = $(Drupal.theme('ModalToolboxDialog'));  
    $('.modal-tooltip-link').removeClass('toggle-on');
    $(element).addClass('toggle-on');
    $(element).parents('tr').addClass('no-opacity');
    // if we already have a modalContent, remove it
    if ( $('#modalContent')) $('#modalContent').remove();
    $('#' + $(element).attr('id') + '-block').html('<div id="modalContent" style="z-index: 1001; position: absolute;">' + $(Drupal.AtriumFolders.ModalToolbox.modal).html() + '</div>');
	}

	/**
	 * Dismiss the modal
	 */
	Drupal.AtriumFolders.ModalToolbox.dismiss = function(element) {
	   $('#modalContent').remove();
	   $('.modal-tooltip-link').removeClass('toggle-on');
     $('.modal-tooltip-link').parents('tr').removeClass('no-opacity');
     $('link.ctools-temporary-css').remove();
	}
	
  /**
   * Provide the HTML to create the modal dialog.
   */
  Drupal.theme.prototype.ModalToolboxDialog = function () {
    var html = ''
    html += '  <div id="ctools-modal">'
    html += '    <div class="ctools-modal-content">' // panels-modal-content
    html += '      <div id="modal-content" class="modal-content">';
    html += '      </div>';
    html += '    </div>';
    html += '  </div>';

    return html;
  }

  /**
   * Generic replacement click handler to open the modal with the destination
   * specified by the href of the link.
   */
  Drupal.AtriumFolders.ModalToolbox.clickAjaxLink = function() {

    if ($(this).hasClass('toggle-on')) {
      Drupal.AtriumFolders.ModalToolbox.dismiss();
      return false;
    }
	  
    Drupal.CTools.AJAX.clickAJAXLink.apply(this);
    Drupal.AtriumFolders.ModalToolbox.show();
    
    if (!$(this).hasClass('ctools-ajaxing')) {
	    Drupal.AtriumFolders.ModalToolbox.dismiss();
	    return false;
    }
    
    return false;
  };

  /**
   * Bind links that will open modals to the appropriate function.
   */
  Drupal.behaviors.ModalToolbox = function(context) {
    // Bind links
    $('a.modal-toolbox:not(.modal-toolbox-processed)', context)
      .addClass('modal-toolbox-processed')
      .click(Drupal.AtriumFolders.ModalToolbox.clickAjaxLink);

    if ($(context).attr('id') == 'modal-content') {
      // Bind submit links in the modal form.
      $('form:not(.modal-toolbox-processed)', context)
        .addClass('modal-toolbox-processed')
        .submit(Drupal.CTools.Modal.submitAjaxForm);
      // add click handlers so that we can tell which button was clicked,
      // because the AJAX submit does not set the values properly.

      $('input[type="submit"]:not(.modal-toolbox-processed), button:not(.modal-toolbox-processed)', context)
        .addClass('modal-toolbox-processed')
        .click(function() {
          if (Drupal.autocompleteSubmit && !Drupal.autocompleteSubmit()) {
            return false;
          }
        });
    }
  };
  

  /**
   * Show modal dialog on ajax call success.
   */
  Drupal.CTools.AJAX.commands.show_modal_dialog = function(command) {
		var element = $('#' + command.id);  
		Drupal.AtriumFolders.ModalToolbox.show(element);
  }  

  /**
   * Dismiss modal dialog on ajax call success.
   */
  Drupal.CTools.AJAX.commands.dismiss_modal_toolbox = function(command) {
    Drupal.AtriumFolders.ModalToolbox.dismiss();
  }  
  
})(jQuery);
