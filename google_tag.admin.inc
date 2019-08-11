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

  $form['intro'] = array(
    '#type' => 'item',
    '#description' => t('<a href="@path">Google Tag Manager</a> is a free service (registration required) to manage the insertion of tags for capturing website analytics.', $args = array('@path' => 'https://tagmanager.google.com/')),
  );

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
  $description = t('The ID assigned by Google Tag Manager (GTM) for this website container. To get a container ID, <a href="@path">sign up for GTM</a> and create a container for your website.', array('@path' => 'https://tagmanager.google.com/'));

  $form['tabs']['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#collapse' => FALSE,
  );

  $form['tabs']['general']['google_tag_container_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Container ID'),
    '#description' => $description,
    '#element_validate' => array('google_tag_validate_container_id'),
    '#attributes' => array('placeholder' => array('GTM-xxxxxx')),
    '#default_value' => $config->get('google_tag_container_id'),
    '#size' => 12,
    '#maxlength' => 15,
    '#required' => TRUE,
  );

  // Path tab
  $description = t('On this and the next two tabs, specify the conditions on which the GTM JavaScript snippet will either be included in or excluded from the page response, thereby enabling or disabling tracking and other analytics.');
  $description .= t(' All conditions must be satisfied for the snippet to be included. The snippet will be excluded if any condition is not met.<br /><br />');
  $description .= t(' On this tab, specify the path condition.');

  $form['tabs']['path'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page paths'),
    '#description' => '<p>' . $description . '</p>',
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
    '#description' => t('Enter one relative path per line using the "*" character as a wildcard. Example paths are: "%blog" for the blog page, "%blog-wildcard" for each individual blog, and "%front" for the front page.', $args = array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
    '#default_value' => $config->get('google_tag_path_list'),
    '#element_validate' => array('google_tag_validate_text'),
    '#rows' => 10,
  );

  // Role tab
  $form['tabs']['role'] = array(
    '#type' => 'fieldset',
    '#title' => t('User roles'),
  );

  $form['tabs']['role']['google_tag_role_toggle'] = array(
    '#type' => 'radios',
    '#title' => t('Add snippet for specific roles'),
    '#options' => array(
      GOOGLE_TAG_EXCLUDE_LISTED => t('All roles except the selected roles'),
      GOOGLE_TAG_INCLUDE_LISTED => t('Only the selected roles'),
    ),
    '#default_value' => $config->get('google_tag_role_toggle'),
  );

  $role_options = array_map('check_plain', user_roles());
  $form['tabs']['role']['google_tag_role_list'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Selected roles'),
    '#default_value' => $config->get('google_tag_role_list'),
    '#options' => $role_options,
  );

  // Status tab
  $form['tabs']['status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Response statuses'),
    '#description' => t('<p>On this tab, specify the page response status condition.</p>'),
  );

  // Build variables.
  $form['tabs']['status']['google_tag_status_toggle'] = array(
    '#type' => 'radios',
    '#title' => t('Add snippet for specific statuses'),
    '#options' => array(
      GOOGLE_TAG_EXCLUDE_LISTED => t('All statuses except the listed statuses'),
      GOOGLE_TAG_INCLUDE_LISTED => t('Only the listed statuses'),
    ),
    '#default_value' => $config->get('google_tag_status_toggle'),
  );

  $form['tabs']['status']['google_tag_status_list'] = array(
    '#type' => 'textarea',
    '#title' => t('Listed statuses'),
    '#description' => t('Enter one response status per line. For more information, refer to the <a href="@path">list of HTTP status codes</a>.', array('@path' => 'http://en.wikipedia.org/wiki/List_of_HTTP_status_codes')),
    '#default_value' => $config->get('google_tag_status_list'),
    '#element_validate' => array('google_tag_validate_text'),
    '#rows' => 5,
  );

  // Advanced tab
  $form['tabs']['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
  );

  // Build variables.
  $form['tabs']['advanced']['google_tag_compact_snippet'] = array(
    '#type' => 'checkbox',
    '#title' => t('Compact the JavaScript snippet'),
    '#description' => t('If checked, then the JavaScript snippet will be compacted to remove unnecessary whitespace. This is <strong>recommended on production sites</strong>. Leave unchecked to output a snippet that can be examined using a JavaScript debugger in the browser.'),
    '#default_value' => $config->get('google_tag_compact_snippet'),
  );

  $form['tabs']['advanced']['google_tag_include_file'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include the snippet as a file'),
    '#description' => t('If checked, then each JavaScript snippet will be included as a file. This is <strong>recommended</strong>. Leave unchecked to inline each snippet into the page. This only applies to data layer and script snippets.'),
    '#default_value' => $config->get('google_tag_include_file'),
  );

  $form['tabs']['advanced']['google_tag_debug_output'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show debug output'),
    '#description' => t('If checked, then the result of each snippet insertion condition will be shown in the message area. Enable <strong>only for development</strong> purposes.'),
    '#default_value' => $config->get('google_tag_debug_output'),
  );

  $disabled = !module_exists('datalayer');
  $markup = t('If the default data layer name is used, then enable the <a href="@path">data layer</a> module to manage the insertion of tags, triggers, and variables in one place. If a different module is used for this purpose, there may be some contamination and override of values from this and the other module.', $args = array('@path' => 'https://www.drupal.org/project/datalayer'));

  $form['tabs']['advanced']['google_tag_data_layer'] = array(
    '#type' => 'textfield',
    '#title' => t('Data layer'),
    '#description' => t('The name of the data layer. Default value is "dataLayer". In most cases, use the default.'),
    '#default_value' => $config->get('google_tag_data_layer'),
    '#element_validate' => array('google_tag_validate_data_layer'),
    '#attributes' => array('placeholder' => array('dataLayer')),
    '#required' => TRUE,
    '#prefix' => $disabled ? '<span>' . $markup . '</span>' : '',
  );

  $form['tabs']['advanced']['google_tag_include_classes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add classes to the data layer'),
    '#description' => t('If checked, then the listed classes will be added to the data layer.'),
    '#default_value' => $config->get('google_tag_include_classes'),
    '#element_validate' => array('google_tag_validate_listed_classes'),
  );

  $form['tabs']['advanced']['google_tag_whitelist_classes'] = array(
    '#type' => 'textarea',
    '#title' => t('White-listed classes'),
    '#description' => t('The types of tags, triggers, and variables <strong>allowed</strong> on a page. Enter one class per line. For more information, refer to the <a href="@path">developer documentation</a>.', $args = array('@path' => 'https://developers.google.com/tag-manager/devguide#security')),
    '#default_value' => $config->get('google_tag_whitelist_classes'),
    '#element_validate' => array('google_tag_validate_text'),
    '#rows' => 5,
    '#states' => _google_tag_variable_states('google_tag_include_classes'),
  );

  $form['tabs']['advanced']['google_tag_blacklist_classes'] = array(
    '#type' => 'textarea',
    '#title' => t('Black-listed classes'),
    '#description' => t('The types of tags, triggers, and variables <strong>forbidden</strong> on a page. Enter one class per line.'),
    '#default_value' => $config->get('google_tag_blacklist_classes'),
    '#element_validate' => array('google_tag_validate_text'),
    '#rows' => 5,
    '#states' => _google_tag_variable_states('google_tag_include_classes'),
  );

  $form['tabs']['advanced']['google_tag_include_environment'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include an environment'),
    '#description' => t('If checked, then the applicable snippets will include the environment items below. Enable <strong>only for development</strong> purposes.'),
    '#default_value' => $config->get('google_tag_include_environment'),
  );

  $form['tabs']['advanced']['google_tag_environment_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Environment ID'),
    '#description' => t('The environment ID to use with this website container. To get an environment ID, <a href="@path">select Environments</a>, create an environment, then click the "Get Snippet" action. The environment ID and token will be in the snippet.', $args = array('@path' => 'https://tagmanager.google.com/#/admin')),
    '#default_value' => $config->get('google_tag_environment_id'),
    '#element_validate' => array('google_tag_validate_environment_id'),
    '#attributes' => array('placeholder' => array('env-x')),
    '#size' => 10,
    '#maxlength' => 7,
    '#states' => _google_tag_variable_states('google_tag_include_environment'),
  );

  $form['tabs']['advanced']['google_tag_environment_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Environment token'),
    '#description' => t('The authentication token for this environment.'),
    '#default_value' => $config->get('google_tag_environment_token'),
    '#attributes' => array('placeholder' => array('xxxxxxxxxxxxxxxxxxxxxx')),
    '#size' => 20,
    '#maxlength' => 25,
    '#states' => _google_tag_variable_states('google_tag_include_environment'),
  );

  // dataLayer variables
  $markup = t('<strong>NOTE</strong>: If using the dataLayer module to insert these values and variable realms are in effect, then <a href="@path">check this item to be realm-specific</a>.', $args = array('@path' => 'admin/config/system/variable/realm/language/configure'));
  $visible = module_exists('datalayer') && module_exists('variable_realm');

  $form['tabs']['advanced']['google_tag_data_layer_classes'] = array(
    '#type' => 'properties',
    '#title' => t('Data layer classes'),
    '#description' => t('The associative array of data layer classes. <strong>NOTE: If using the dataLayer module to insert these values and variable realms are in effect, then check this item.</strong>'),
    '#default_value' => $config->get('google_tag_data_layer_classes'),
//    '#prefix' => $visible ? '<span>' . $markup . '</span>' : '',
    '#attributes' => array('class' => array('element-invisible')),
  );

  // Submit
  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form validation handler for google_tag_settings_form().
 */
function google_tag_settings_form_validate($form, &$form_state) {
  $values = &$form_state['values'];

  // Trim the text values.
  $values['google_tag_container_id'] = trim($values['google_tag_container_id']);
  $values['google_tag_data_layer'] = trim($values['google_tag_data_layer']);
  google_tag_text_clean($values['google_tag_path_list']);
  google_tag_text_clean($values['google_tag_status_list']);
  google_tag_text_clean($values['google_tag_whitelist_classes']);
  google_tag_text_clean($values['google_tag_blacklist_classes']);

  // Replace all types of dashes (n-dash, m-dash, minus) with a normal dash.
  $values['google_tag_container_id'] = str_replace(array('–', '—', '−'), '-', $values['google_tag_container_id']);
  $values['google_tag_environment_id'] = str_replace(array('–', '—', '−'), '-', $values['google_tag_environment_id']);

  if (!preg_match('/^GTM-\w{4,}$/', $values['google_tag_container_id'])) {
    // @todo Is there a more specific regular expression that applies?
    // @todo Is there a way to "test the connection" to determine a valid ID for
    // a container? It may be valid but not the correct one for the website.
    form_set_error('google_tag_container_id', t('A valid container ID is case sensitive and formatted like GTM-xxxxxx.'));
  }
  if ($values['google_tag_include_environment'] && !preg_match('/^env-\d{1,}$/', $values['google_tag_environment_id'])) {
    form_set_error('google_tag_environment_id', t('A valid environment ID is case sensitive and formatted like env-x.'));
  }
  if ($message = _google_tag_data_layer_verify($values['google_tag_data_layer'])) {
    form_set_error('google_tag_data_layer', $message);
  }
  if ($values['google_tag_include_classes']) {
    if (empty($values['google_tag_whitelist_classes']) && empty($values['google_tag_blacklist_classes'])) {
      form_set_error('google_tag_include_classes', t('Enter listed classes in at least one field, or uncheck the box.'));
      form_set_error('google_tag_whitelist_classes', '');
      form_set_error('google_tag_blacklist_classes', '');
    }
  }
}

/**
 * Form submission handler for google_tag_settings_form().
 */
function google_tag_settings_form_submit($form, &$form_state) {
  $config = config('google_tag.settings');
  $config->set('google_tag_container_id', $form_state['values']['google_tag_container_id']);
  $config->set('google_tag_path_toggle', $form_state['values']['google_tag_path_toggle']);
  $config->set('google_tag_path_list', $form_state['values']['google_tag_path_list']);
  $config->set('google_tag_role_toggle', $form_state['values']['google_tag_role_toggle']);
  $config->set('google_tag_role_list', $form_state['values']['google_tag_role_list']);
  $config->set('google_tag_status_toggle', $form_state['values']['google_tag_status_toggle']);
  $config->set('google_tag_status_list', $form_state['values']['google_tag_status_list']);
  $config->set('google_tag_compact_snippet', (bool) $form_state['values']['google_tag_compact_snippet']);
  $config->set('google_tag_include_file', (bool) $form_state['values']['google_tag_include_file']);
  $config->set('google_tag_debug_output', (bool) $form_state['values']['google_tag_debug_output']);
  $config->set('google_tag_data_layer', $form_state['values']['google_tag_data_layer']);
  $config->set('google_tag_include_classes', (bool) $form_state['values']['google_tag_include_classes']);
  $config->set('google_tag_whitelist_classes', $form_state['values']['google_tag_whitelist_classes']);
  $config->set('google_tag_blacklist_classes', $form_state['values']['google_tag_blacklist_classes']);
  $config->set('google_tag_include_environment', $form_state['values']['google_tag_include_environment']);
  $config->set('google_tag_environment_id', $form_state['values']['google_tag_environment_id']);
  $config->set('google_tag_environment_token', $form_state['values']['google_tag_environment_token']);
  $config->set('google_tag_data_layer_classes', $form_state['values']['google_tag_data_layer_classes']);

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

/**
 * Element validation handler for google_tag_container_id.
 *
 * @todo This may suffice for all the elements as only the container_id has an
 *   error message. Error messages could be done for other elements by calling
 *   form_set_error('element[name]', $message).
 */
function google_tag_validate_container_id($element, &$form_state) {
//  // Use this routine as the variable realm form has nested values.
//  $value = backdrop_array_get_nested_value($form_state['values'], $element['#parents']);
//  // Trim the text value.
//  $value = trim($value);
//  // Replace all types of dashes (n-dash, m-dash, minus) with a normal dash.
//  $value = str_replace(array('–', '—', '−'), '-', $value);
//  // Replace the text value.
//  backdrop_array_set_nested_value($form_state['values'], $element['#parents'], $value);
//
//  if (!preg_match('/^GTM-\w{4,}$/', $value)) {
//    // @todo Is there a more specific regular expression that applies?
//    // @todo Is there a way to "test the connection" to determine a valid ID for
//    // a container? It may be valid but not the correct one for the website.
//    return t('A valid container ID is case sensitive and formatted like GTM-xxxxxx.');
//  }
}

/**
 * Element validation handler for google_tag_environment_id.
 */
function google_tag_validate_environment_id($element, $form, &$form_state) {
//  // @todo This will not prevent checking 'include_environment' on an individual
//  // variable edit form and never setting the 'environment_id'.
//  $parents = $element['#parents'];
//  array_pop($parents);
//  $parents[] = 'google_tag_include_environment';
//  $include_environment = backdrop_array_get_nested_value($form_state['values'], $parents);
//
//  $value = backdrop_array_get_nested_value($form_state['values'], $element['#parents']);
//  $value = trim($value);
//  $value = str_replace(array('–', '—', '−'), '-', $value);
//  backdrop_array_set_nested_value($form_state['values'], $element['#parents'], $value);
//
//  if ($include_environment && !preg_match('/^env-\d{1,}$/', $value)) {
//    return t('A valid environment ID is case sensitive and formatted like env-x.');
//  }
}

/**
 * Element validation handler for google_tag_data_layer.
 */
function google_tag_validate_data_layer($element, $form, &$form_state) {
//  $value = backdrop_array_get_nested_value($form_state['values'], $element['#parents']);
//  $value = trim($value);
//  backdrop_array_set_nested_value($form_state['values'], $element['#parents'], $value);
//  return _google_tag_data_layer_verify($value);
}

/**
 * Element validation handler for google_tag_listed_classes.
 */
function google_tag_validate_listed_classes($element, $form, &$form_state) {
//  if (!backdrop_array_get_nested_value($form_state['values'], $element['#parents'])) {
//    return;
//  }
//
//  $keys = array('google_tag_whitelist_classes', 'google_tag_blacklist_classes');
//  $variables = isset($form['#variable_edit_form']) ? $form['#variable_edit_form'] : array();
//  $items = array_diff($keys, $variables);
//  if (!empty($items)) {
//    // Need both variables on form to validate.
//    form_set_error('google_tag_include_classes', t('To validate listed classes, include the whitelist and blacklist variables in the realm variables.'));
//    return;
//  }
//
//  $empty = TRUE;
//  foreach ($keys as $key) {
//    array_pop($element['#parents']);
//    $element['#parents'][] = $key;
//    $classes = backdrop_array_get_nested_value($form_state['values'], $element['#parents']);
//    google_tag_text_clean($classes);
//    $empty &= empty($classes);
//  }
//  if ($empty) {
//    form_set_error('google_tag_include_classes', t('Enter listed classes in at least one field, or uncheck the box.'));
//    form_set_error('google_tag_whitelist_classes', '');
//    form_set_error('google_tag_blacklist_classes', '');
//  }
}

/**
 * Element validation handler for text values.
 */
function google_tag_validate_text($element, $form, &$form_state) {
//  $value = backdrop_array_get_nested_value($form_state['values'], $element['#parents']);
//  google_tag_text_clean($value);
//  backdrop_array_set_nested_value($form_state['values'], $element['#parents'], $value);
}

/**
 * Returns states array for a form element.
 *
 * @param string $variable
 *   The name of the form element.
 *
 * @return array
 *   The states array.
 */
function _google_tag_variable_states($variable) {
  return array(
    'required' => array(
      ':input[name="' . $variable . '"]' => array('checked' => TRUE),
    ),
    'invisible' => array(
      ':input[name="' . $variable . '"]' => array('checked' => FALSE),
    ),
  );
}
