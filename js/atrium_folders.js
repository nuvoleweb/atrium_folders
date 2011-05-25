/**
 * @file
 * Implement a modal toolbox.
 */

(function ($) {

	/**
	 * Display the modal
	 */
	Drupal.CTools.Modal.showToolbox = function(element) {

	  if ($(element).hasClass('toggle-on')) {
		  $('#modalContent').remove();
		  $('.modal-tooltip-link').removeClass('toggle-on');
		  return false;
	  }

	  Drupal.CTools.Modal.modal = $(Drupal.theme('CToolsModalToolboxDialog'));  
	  $('.modal-tooltip-link').removeClass('toggle-on');
	  $(element).addClass('toggle-on');
	  
	  // if we already have a modalContent, remove it
	  if ( $('#modalContent')) $('#modalContent').remove();
	  $('#' + $(element).attr('id') + '-block').html('<div id="modalContent" style="z-index: 1001; position: absolute;">' + $(Drupal.CTools.Modal.modal).html() + '</div>');
	}

	  /**
	   * Provide the HTML to create the modal dialog.
	   */
	  Drupal.theme.prototype.CToolsModalToolboxDialog = function () {
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
	  Drupal.CTools.Modal.clickAjaxLink = function() {
	    return false;
	  };
		
	  /**
	   * Show modal dialog on ajax call success.
	   */
	  Drupal.CTools.AJAX.commands.show_modal_dialog = function(command) {
		var element = $('#' + command.id);  
		Drupal.CTools.Modal.showToolbox(element);
	  }  

})(jQuery);
