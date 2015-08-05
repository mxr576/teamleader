<?php
/**
 * @file
 * Teamleader administration pages.
 */

/**
 * Teamleader API settings form.
 */
function teamleader_admin_api_settings($form, &$form_state) {
  $form[TEAMLEADER_API_GROUP] = array(
    '#type' => 'textfield',
    '#title' => t('API group'),
    '#default_value' => variable_get(TEAMLEADER_API_GROUP, ''),
    '#required' => TRUE,
  );
  $form[TEAMLEADER_API_SECRET] = array(
    '#type' => 'textarea',
    '#title' => t('API secret'),
    '#default_value' => variable_get(TEAMLEADER_API_SECRET, ''),
    '#required' => TRUE,
    '#description' => t('You can find your Teamleader API connection information <a href="@url" target="_blank">here</a>.', array('@url' => url('https://www.teamleader.be/apiwebhooks.php?show_key', array('external' => TRUE)))),
  );
  $form['teamleader_validate_connection'] = array(
    '#type' => 'checkbox',
    '#title' => t('Validate connection'),
    '#description' => t('Validate settings, when the configuration saved.'),
    '#default_value' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Validation handler for teamleader_admin_api_settings form.
 *
 * @see teamleader_admin_api_settings()
 */
function teamleader_admin_api_settings_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['teamleader_api_group'])) {
    form_set_error('teamleader_api_group', t('Teamleader API group should contains only numeric values!'));
  }

  if (teamleader_test_connection($form_state['values']['teamleader_api_group'], $form_state['values']['teamleader_api_secret'])) {
    drupal_set_message(t('Successful Teamleader API connection!'));
  }
  else {
    form_set_error('teamleader_validate_connection', t('Teamleader API connection failed! Check your API credentials!'));
  }
}
