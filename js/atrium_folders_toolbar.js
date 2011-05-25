

Drupal.behaviors.atrium_folders_toolbar = function(context) {
	
	$('a.folder-toolbar-link:not(.folder-toolbar-processed)', context)
    .addClass('folder-toolbar-processed')
    .click(function(event) {
    	event.preventDefault();
    	var id = '#' + $(this).attr('href').split("#")[1];
//    	window.location = $(this).attr("href");
    	$('#node-form').attr('action', $(this).attr("href"));

    	if($(id).hasClass('expanded')) {
    		$(id).slideUp().removeClass('expanded');
    		$(this).removeClass('toggle-on');
    	}
    	else {
    		if ($('.folder-toolbar-fieldset.expanded').length) {
            	$('.folder-toolbar-fieldset.expanded').hide().removeClass('expanded');
            	$('.folder-toolbar-link.toggle-on').removeClass('toggle-on');
        		$(id).show().addClass('expanded');    	
    		}
    		else {
        		$(id).slideDown().addClass('expanded');    	
    		}
    		$(this).addClass('toggle-on');
    	}
    });

	var url = document.location.toString();
	if (url.match('#') && $('.form-item > [class*="error"]').length) { 
	  var anchor = '#' + url.split('#')[1];
	  $('a.folder-toolbar-link[href*="' + anchor + '"]').click();
	}
	
};

