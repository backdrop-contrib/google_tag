<?php

/**
 * @file
 * Contains the administrative page and form callbacks.
 *
 * @author Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Form constructor for the module settings form.
 *
 * @see google_tag_settings_form_validate()
 * @see google_tag_settings_form_submit()
 *
 * @ingroup forms
 */
function google_tag_settings_form($form, &$form_state) {
  $config = config('google_tag.settings');
//  module_load_include('inc', 'google_tag', 'includes/variable');

  // Gather data.
  // @todo Set this on validation errors.
  $default_tab = !empty($form_state['default_tab']) ? $form_state['default_tab'] : '';

  $description = t('On this and the next two tabs, specify the conditions on which the GTM JavaScript snippet will either be included in or excluded from the page response, thereby enabling or disabling tracking and other analytics.');
  $description .= t(' All conditions must be satisfied for the snippet to be included. The snippet will be excluded if any condition is not met.<br /><br />');
  $description .= t(' On this tab, specify the path condition.');

  $form['tabs'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'css' => array(backdrop_get_path('module', 'google_tag') . '/css/google_tag.admin.css'),
      'js' => array(backdrop_get_path('module', 'google_tag') . '/js/google_tag.admin.js'),
    ),
  );

  // General tab
  $args = array('@path' => 'https://tagmanager.google.com/');
  $description = t('The ID assigned by Google Tag Manager (GTM) for this website container. To get a container ID, <a href="@path">sign up for GTM</a> and create a container for your website.', $args);

  $form['tabs']['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#collapse' => FALSE,
  );

  $form['tabs']['general']['google_tag_container_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Container ID'),
    '#description' => $description,
    '#validate callback' => '_google_tag_validate_container_id',
    '#attributes' => array('placeholder' => array('GTM-xxxxxx')),
    '#default_value' => $config->get('google_tag_container_id'),
    '#size' => 12,
    '#maxlength' => 15,
    '#required' => TRUE,
  );

  // Paths tab
  $args = array(
    '%blog' => 'blog',
    '%blog-wildcard' => 'blog/*',
    '%front' => '<front>',
  );
  $list_description = t('Enter one relative path per line using the "*" character as a wildcard. Example paths are: "%blog" for the blog page, "%blog-wildcard" for each individual blog, and "%front" for the front page.', $args);

  $form['tabs']['path'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page paths'),
  );

  $form['tabs']['path']['google_tag_path_toggle'] = array(
    '#type' => 'radios',
    '#title' => t('Add snippet on specific paths'),
    '#options' => array(
      GOOGLE_TAG_EXCLUDE_LISTED => t('All paths except the listed paths'),
      GOOGLE_TAG_INCLUDE_LISTED => t('Only the listed paths'),
    ),
    '#default_value' => $config->get('google_tag_path_toggle'),
  );

  $form['tabs']['path']['google_tag_path_list'] = array(
    '#type' => 'textarea',
    '#title' => t('Listed paths'),
    '#description' => $list_description,
    '#default_value' => $config->get('google_tag_path_list'),
    '#validate callback' => '_google_tag_validate_text',
    '#rows' => 10,
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );


//  $groups = array(
//    'general' => array('title' => t('General'), 'collapse' => FALSE),
//    'path' => array('title' => t('Page paths'), 'description' => $description),
//    'role' => array('title' => t('User roles'), 'description' => t('On this tab, specify the user role condition.')),
//    'status' => array('title' => t('Response statuses'), 'description' => t('On this tab, specify the page response status condition.')),
//    'advanced' => array('title' => t('Advanced')),
//  );

  // Build form elements.
//  $form['tabs'] = array(
//    '#type' => 'vertical_tabs',
//    '#default_tab' => $default_tab ? $default_tab : 'edit-general',
//    '#attributes' => array('class' => array('google-tag')),
//    '#attached' => array(
//      'css' => array(backdrop_get_path('module', 'google_tag') . '/css/google_tag.admin.css'),
//      'js' => array(backdrop_get_path('module', 'google_tag') . '/js/google_tag.admin.js'),
//    ),
//  );
//  foreach ($groups as $group => $items) {
//    $form['tabs'][$group] = google_tag_fieldset($group, $items);
//  }


//  $form['#after_build'] = array('google_tag_settings_form_after_build');

  return $form;
}

/**
 * Fieldset builder for the module settings form.
 */
function google_tag_fieldset($group, $items) {
  // Gather data.
//  $function = "_google_tag_variable_info_$group";
//  $variables = $function(array());
//  $items += array('description' => '', 'collapse' => TRUE);
//
//  // Build form elements.
//  $fieldset = array(
//    '#type' => 'fieldset',
//    '#title' => $items['title'],
//    '#description' => $items['description'],
//    '#collapsible' => $items['collapse'],
//    '#collapsed' => $items['collapse'],
//    '#tree' => FALSE,
//  );
//  $fieldset += google_tag_form_elements($variables);
//
//  return $fieldset;
}

/**
 * Returns form elements from variable definitions.
 *
 * @param array $variables
 *   Associative array of variable definitions.
 *
 * @return array
 *   Associative array of form elements.
 */
function google_tag_form_elements($variables) {
//  static $keys = array(
//    'type' => '#type',
//    'title' => '#title',
//    'description' => '#description',
//    'options' => '#options',
//// @todo is it safe to just kill this since we will read from confg?
////    'default' => '#default_value',
//  );
//  $elements = array();
//  foreach ($variables as $name => $variable) {
//    $element = array();
//    foreach ($keys as $key => $property) {
//      if (isset($variable[$key])) {
//        $element[$property] = $variable[$key];
//      }
//    }
//    $element['#type'] = google_tag_form_element_type($element['#type']);
////    @todo is it safe to just kill this since we will read from config?
////    $element['#default_value'] = config_get('google_tag.settings',$name, $variable['default']);
//    $element += isset($variable['element']) ? $variable['element'] : array();
//    $elements[$name] = $element;
//  }
//
//  return $elements;
}

/**
 * Converts variable type to form element type.
 *
 * @param string $type
 *   Variable type.
 *
 * @return string
 *   Form element type.
 */
function google_tag_form_element_type($type) {
//  static $keys = array(
//    'string' => 'textfield',
//    'select' => 'radios',
//    'text' => 'textarea',
//    'options' => 'checkboxes',
//    'boolean' => 'checkbox',
//  );
//  return isset($keys[$type]) ? $keys[$type] : 'textfield';
}

/**
 * Element after build callback for google_tag_settings_form().
 */
function google_tag_settings_form_after_build($element, &$form_state) {
//  // This will follow:
//  // variable_realm_variable_settings_form_submit
//  // variable_settings_form_submit
//  $element['#submit'][] = 'google_tag_settings_form_submit';
//  return $element;
}

/**
 * Form validation handler for google_tag_settings_form().
 */
function google_tag_settings_form_validate($form, &$form_state) {
//  $values = &$form_state['values'];
//
//  // Trim the text values.
//  $values['google_tag_container_id'] = trim($values['google_tag_container_id']);
//  $values['google_tag_data_layer'] = trim($values['google_tag_data_layer']);
//  google_tag_text_clean($values['google_tag_path_list']);
//  google_tag_text_clean($values['google_tag_status_list']);
//  google_tag_text_clean($values['google_tag_whitelist_classes']);
//  google_tag_text_clean($values['google_tag_blacklist_classes']);
//
//  // Replace all types of dashes (n-dash, m-dash, minus) with a normal dash.
//  $values['google_tag_container_id'] = str_replace(array('–', '—', '−'), '-', $values['google_tag_container_id']);
//  $values['google_tag_environment_id'] = str_replace(array('–', '—', '−'), '-', $values['google_tag_environment_id']);
//
//  if (!preg_match('/^GTM-\w{4,}$/', $values['google_tag_container_id'])) {
//    // @todo Is there a more specific regular expression that applies?
//    // @todo Is there a way to "test the connection" to determine a valid ID for
//    // a container? It may be valid but not the correct one for the website.
//    form_set_error('google_tag_container_id', t('A valid container ID is case sensitive and formatted like GTM-xxxxxx.'));
//  }
//  if ($values['google_tag_include_environment'] && !preg_match('/^env-\d{1,}$/', $values['google_tag_environment_id'])) {
//    form_set_error('google_tag_environment_id', t('A valid environment ID is case sensitive and formatted like env-x.'));
//  }
//  if ($message = _google_tag_data_layer_verify($values['google_tag_data_layer'])) {
//    form_set_error('google_tag_data_layer', $message);
//  }
//  if ($values['google_tag_include_classes']) {
//    if (empty($values['google_tag_whitelist_classes']) && empty($values['google_tag_blacklist_classes'])) {
//      form_set_error('google_tag_include_classes', t('Enter listed classes in at least one field, or uncheck the box.'));
//      form_set_error('google_tag_whitelist_classes', '');
//      form_set_error('google_tag_blacklist_classes', '');
//    }
//  }
}

/**
 * Form submission handler for google_tag_settings_form().
 */
function google_tag_settings_form_submit($form, &$form_state) {
  $config = config('google_tag.settings');
  $config->set('google_tag_container_id', $form_state['values']['google_tag_container_id']);
  $config->set('google_tag_path_toggle', $form_state['values']['google_tag_path_toggle']);
  $config->set('google_tag_path_list', $form_state['values']['google_tag_path_list']);

  //
  //@todo all of the variables
  // examples from googleanalytics

  //  $config->set('account', $form_state['values']['googleanalytics_account']);
  //  $config->set('domain_mode', (int) $form_state['values']['googleanalytics_domain_mode']);
  //  $config->set('cross_domains', $form_state['values']['googleanalytics_cross_domains']);
  //  $config->set('visibility_pages', (int) $form_state['values']['googleanalytics_visibility_pages']);
  //  $config->set('pages', $form_state['values']['googleanalytics_pages']);
  //  $config->set('visibility_roles', (int) $form_state['values']['googleanalytics_visibility_roles']);
  //  $config->set('roles', array_keys(array_filter($form_state['values']['googleanalytics_roles'])));
  //  $config->set('custom', $form_state['values']['googleanalytics_custom']);
  //  $config->set('trackuserid', (bool) $form_state['values']['googleanalytics_trackuserid']);
  //  $config->set('trackoutbound', (bool) $form_state['values']['googleanalytics_trackoutbound']);
  //  $config->set('trackmailto', (bool) $form_state['values']['googleanalytics_trackmailto']);
  //  $config->set('trackfiles', (bool) $form_state['values']['googleanalytics_trackfiles']);
  //  $config->set('trackfiles_extensions', $form_state['values']['googleanalytics_trackfiles_extensions']);
  //  $config->set('tracklinkid', (bool) $form_state['values']['googleanalytics_tracklinkid']);
  //  $config->set('trackurlfragments', (bool) $form_state['values']['googleanalytics_trackurlfragments']);
  //  $config->set('trackmessages', $form_state['values']['googleanalytics_trackmessages']);
  //  $config->set('site_search', (bool) $form_state['values']['googleanalytics_site_search']);
  //  $config->set('trackadsense', (bool) $form_state['values']['googleanalytics_trackadsense']);
  //  $config->set('trackdoubleclick', (bool) $form_state['values']['googleanalytics_trackdoubleclick']);
  //  $config->set('tracker_anonymizeip', (bool) $form_state['values']['googleanalytics_tracker_anonymizeip']);
  //  $config->set('privacy_donottrack', (bool) $form_state['values']['googleanalytics_privacy_donottrack']);
  //  $config->set('custom_dimension', $form_state['values']['googleanalytics_custom_dimension']);
  //  $config->set('custom_metric', $form_state['values']['googleanalytics_custom_metric']);
  //  $config->set('cache', (bool) $form_state['values']['googleanalytics_cache']);
  //  $config->set('codesnippet_create', $form_state['values']['googleanalytics_codesnippet_create']);
  //  $config->set('codesnippet_before', $form_state['values']['googleanalytics_codesnippet_before']);
  //  $config->set('codesnippet_after', $form_state['values']['googleanalytics_codesnippet_after']);
  //  $config->set('debug', (bool) $form_state['values']['googleanalytics_debug']);

  $config->save();

  backdrop_set_message(t('The configuration options have been saved.'));
}

/**
 * Saves snippet files and data layer classes based on current settings.
 *
 * Use case: variable_realm is not enabled
 * - one set of snippet files stored in public://google_tag
 *
 * Use case: variable_realm is enabled
 * - multiple sets of snippet files stored in public://google_tag/{realm}
 * - sub case: no google_tag variables are realm-specific
 *   update all snippet files based on global default variables
 * - sub case: some google_tag variables are realm-specific
 *   update all snippet files based on global and realm-specific variables
 */
function _google_tag_assets_create() {
  if (module_exists('variable_realm') && module_exists('variable_store')) {
    // i18n_variable module depends on variable_realm, variable_store
    $realms = variable_realm_list();
    foreach ($realms as $realm_name => $realm_title) {
      $keys = variable_realm_keys($realm_name);
      foreach ($keys as $key_name => $key_title) {
        backdrop_set_message(t('realm:key = @realm:@key', array('@realm' => $realm_name, '@key' => $key_name)));
        variable_realm_switch($realm_name, $key_name);
        if (_google_tag_directory_prepare($realm_name)) {
          _google_tag_snippets_save($realm_name, $key_name);
          _google_tag_classes_save($realm_name, $key_name);
        }
      }
    }
  }
  else {
    if (_google_tag_directory_prepare()) {
      _google_tag_snippets_save();
      _google_tag_classes_save();
    }
  }

  _backdrop_flush_css_js();
  backdrop_clear_js_cache();
}

/**
 * Prepares directory for base or realm specific snippet files.
 *
 * @return bool
 *   Whether the directory was prepared.
 */
function _google_tag_directory_prepare($realm_name = '') {
  // From google_tag_requirements(); this should be a helper function (in core).
  $directory = 'public://google_tag';
  $directory .= $realm_name ? "/$realm_name" : '';
  if (is_dir($directory) && _google_tag_is_writable($directory) && _google_tag_is_executable($directory)) {
    return TRUE;
  }
  if (__file_prepare_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
    return TRUE;
  }

  // The snippet directory does not exist or is not writable or searchable.

  // If applicable, get the directory path of stream wrapper.
  $wrapper = file_stream_wrapper_get_instance_by_uri($directory);
  if (method_exists($wrapper, 'getDirectoryPath') && ($path = $wrapper->getDirectoryPath())) {
    // getDirectoryPath() is not defined in StreamWrapperInterface; it
    // exists in LocalStream and the local storage replacement classes in
    // google_appengine; s3fs returns an empty string.
    $path .= str_replace('public://', '/', $directory);
  }
  elseif (!($path = $wrapper->getExternalUrl())) {
    $path = $directory;
  }

  $args = array('%directory' => $path);
  $message = 'The directory %directory could not be prepared for use, possibly due to file system permissions. The directory either does not exist, or is not writable or searchable.';
  _google_tag_message_display($message, $args, 'error');
  watchdog('google_tag', $message, $args, WATCHDOG_ERROR);
  return FALSE;
}

/**
 * Saves JS snippet files based on current settings.
 *
 * @return bool
 *   Whether the files were saved.
 */
function _google_tag_snippets_save($realm_name = '', $realm_key = '') {
  $realm_string = $realm_name ? "$realm_name:$realm_key" : 'default';
  $realm_name .= $realm_name ? '/' : '';
  $realm_key .= $realm_key ? '.' : '';
  // Save the altered snippets after hook_google_tag_snippets_alter().
  module_load_include('inc', 'google_tag', 'includes/snippet');
  $result = TRUE;
  $snippets = google_tag_snippets();
  foreach ($snippets as $type => $snippet) {
    $path = file_unmanaged_save_data($snippet, "public://google_tag/{$realm_name}google_tag.$realm_key$type.js", FILE_EXISTS_REPLACE);
    $result = !$path ? FALSE : $result;
  }

  $args = array('@count' => count($snippets));
  if ($realm_name) {
    $args += array('@realm' => $realm_string);
    $message = $result
      ? 'Created @count snippet files for @realm realm based on configuration.'
      : 'An error occurred saving @count snippet files for @realm realm. Contact the site administrator if this persists.';
  }
  else {
    $message = $result
      ? 'Created @count snippet files based on configuration.'
      : 'An error occurred saving @count snippet files. Contact the site administrator if this persists.';
  }
  _google_tag_message_display($message, $args, $result ? 'status' : 'error');
  if (!$result) {
    watchdog('google_tag', $message, $args, WATCHDOG_ERROR);
  }
}

/**
 * Stores data layer classes based on current settings.
 */
function _google_tag_classes_save($realm_name = '', $realm_key = '') {
  $data_layer = config_get('google_tag.settings','google_tag_data_layer');
  if (module_exists('datalayer') && $data_layer == 'dataLayer') {
    // Save classes for data layer.
    // @todo How to or can we set the name of the data layer?
    // This is an open issue on datalayer project; after it is implemented in
    // datalayer then remove second condition on if block.
    _google_tag_data_layer_snippet($classes);
    if ($realm_name) {
      variable_realm_set($realm_name, $realm_key, 'google_tag_data_layer_classes', $classes);
    }
    else {
      config_set('google_tag.settings','google_tag_data_layer_classes', $classes);
    }
  }
}

/**
 * Cleans a string representing a list of items.
 *
 * @param string $text
 *   The string to clean.
 * @param string $format
 *   The final format of $text, either 'string' or 'array'.
 */
function google_tag_text_clean(&$text, $format = 'string') {
  $text = explode("\n", $text);
  $text = array_map('trim', $text);
  $text = array_filter($text, 'trim');
  if ($format == 'string') {
    $text = implode("\n", $text);
  }
}

/**
 * Verifies presence of dataLayer module and compares name of data layer.
 *
 * @return bool|null
 *   Whether data layer name is incompatible with dataLayer module, if present.
 */
function _google_tag_data_layer_verify($value) {
  if (module_exists('datalayer') && $value != 'dataLayer') {
    // @todo Setting form error does not allow user to save a different name,
    // i.e. to do what message text says.
    return t('The 1.1 release of the dataLayer module does not support a data layer name other than "dataLayer." If you need a different layer name, then either disable the dataLayer module or alter the JavaScript added to the page response.');
  }
}

/**
 * Checks that the directory exists and is writable.
 *
 * @todo Remove this function if core is updated to check the executable bit.
 *
 * @see file_prepare_directory()
 */
function __file_prepare_directory(&$directory, $options = FILE_MODIFY_PERMISSIONS) {
  if (!file_stream_wrapper_valid_scheme(file_uri_scheme($directory))) {
    // Only trim if we're not dealing with a stream.
    $directory = rtrim($directory, '/\\');
  }

  // Check if directory exists.
  if (!is_dir($directory)) {
    // Let mkdir() recursively create directories and use the default directory
    // permissions.
    if (($options & FILE_CREATE_DIRECTORY) && @backdrop_mkdir($directory, NULL, TRUE)) {
      return backdrop_chmod($directory);
    }
    return FALSE;
  }
  // The directory exists, so check to see if it is writable.
  $writable = _google_tag_is_writable($directory) && _google_tag_is_executable($directory);
  if (!$writable && ($options & FILE_MODIFY_PERMISSIONS)) {
    return backdrop_chmod($directory);
  }

  return $writable;
}

/**
 * Determines whether a directory is writable.
 *
 * Remove this if PHP is_writable() is changed to respect ACLS on a 'local'
 * stream wrapper other than the local file wrapper provided by PHP.
 *
 * @param string $uri
 *   A directory path or stream wrapper URI.
 *
 * @return bool
 *   Whether the directory is writable.
 */
function _google_tag_is_writable($uri) {
  // Use the local path, if applicable, since PHP only checks ACLs on its local
  // file wrapper.
  $realpath = FALSE;
  if ($wrapper = file_stream_wrapper_get_instance_by_uri($uri)) {
    $realpath = $wrapper->realpath($uri);
  }
  return is_writable($realpath ? $realpath : $uri);
}

/**
 * Determines whether a directory is searchable.
 *
 * Remove this if PHP is_executable() is changed to not return FALSE simply
 * because the URI points to a directory (not a file) in a stream wrapper other
 * than the local file wrapper provided by PHP.
 *
 * @param string $uri
 *   A directory path or stream wrapper URI.
 *
 * @return bool
 *   Whether the directory is searchable.
 */
function _google_tag_is_executable($uri) {
  if ($wrapper = file_stream_wrapper_get_instance_by_uri($uri)) {
    if ($realpath = $wrapper->realpath($uri)) {
      // The URI is a local stream wrapper.
      // Use local path since PHP only checks ACLs on its local file wrapper.
      // Remove the OS check if PHP is_executable() is changed to not return FALSE
      // simply because the URI points to a directory (not a file) on Windows.
      return _google_tag_is_windows() || is_executable($realpath);
    }

    // The URI is a remote stream wrapper.
    if (!($stat = $wrapper->url_stat($uri, 0))) {
      return FALSE;
    }

    if (!function_exists('posix_getuid') || !function_exists('posix_getgid')) {
      // These functions are never defined on Windows and the extension that
      // provides them may not be included on a Linux distribution.
      // If directory is not searchable, then fault the site deployment process.
      // @todo Is it worse to return true or false at this point?
      return TRUE;
    }

    // Determine the appropriate permissions bit mask as an octal.
    // The stat array is likely to have uid=gid=0 so that the mask is octal 01.
    // This is true for Amazon S3 and Google Cloud Storage.
    $mask = 1;
    if ($stat['uid'] == posix_getuid()) {
      $mask = $mask << 6;
    }
    elseif ($stat['gid'] == posix_getgid()) {
      $mask = $mask << 3;
    }
    return ($stat['mode'] & $mask) != 0;
  }
  else {
    // The URI is a local path.
    return is_executable($uri);
  }
}

/**
 * Determines whether the operating system is Windows.
 *
 * @return bool
 *   Whether the operating system is Windows.
 */
function _google_tag_is_windows() {
  return (defined('PHP_OS_FAMILY') && PHP_OS_FAMILY == 'Windows') ||
    (defined('PHP_OS') && strcasecmp(substr(PHP_OS, 0, 3), 'win') == 0);
}

/**
 * Displays a message to admin users.
 *
 * @see arguments to t() and backdrop_set_message()
 */
function _google_tag_message_display($message, $args = array(), $type = 'status') {
  global $google_tag_display_message;
  if ($google_tag_display_message) {
    backdrop_set_message(t($message, $args), $type);
  }
}
