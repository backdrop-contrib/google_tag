/**
 * @file
 * Behaviors and utility functions for administrative pages.
 */

(function ($) {

/**
 * Provides summary information for the vertical tabs.
 */
Backdrop.behaviors.gtmInsertionSettings = {
  attach: function (context) {
    if (typeof jQuery.fn.backdropSetSummary == 'undefined') {
      // This behavior only applies if backdropSetSummary is defined.
      return;
    }

    $('fieldset#edit-path', context).backdropSetSummary(function (context) {
      var $radio = $('input[name="google_tag_path_toggle"]:checked', context);
      if ($radio.val() == 'exclude listed') {
        if (!$('textarea[name="google_tag_path_list"]', context).val()) {
          return Backdrop.t('All paths');
        }
        else {
          return Backdrop.t('All paths except listed paths');
        }
      }
      else {
        if (!$('textarea[name="google_tag_path_list"]', context).val()) {
          return Backdrop.t('No paths');
        }
        else {
          return Backdrop.t('Only listed paths');
        }
      }
    });

    $('fieldset#edit-role', context).backdropSetSummary(function (context) {
      var vals = [];
      $('input[type="checkbox"]:checked', context).each(function () {
        vals.push($(this).next('label').text().trim());
      });
      var $radio = $('input[name="google_tag_role_toggle"]:checked', context);
      if ($radio.val() == 'exclude listed') {
        if (!vals.length) {
          return Backdrop.t('All roles');
        }
        else {
          return Backdrop.t('All roles except selected roles');
        }
      }
      else {
        if (!vals.length) {
          return Backdrop.t('No roles');
        }
        else {
          return Backdrop.t('Only selected roles');
        }
      }
    });

    $('fieldset#edit-status', context).backdropSetSummary(function (context) {
      var $radio = $('input[name="google_tag_status_toggle"]:checked', context);
      if ($radio.val() == 'exclude listed') {
        if (!$('textarea[name="google_tag_status_list"]', context).val()) {
          return Backdrop.t('All statuses');
        }
        else {
          return Backdrop.t('All statuses except listed statuses');
        }
      }
      else {
        if (!$('textarea[name="google_tag_status_list"]', context).val()) {
          return Backdrop.t('No statuses');
        }
        else {
          return Backdrop.t('Only listed statuses');
        }
      }
    });
  }
};

})(jQuery);
