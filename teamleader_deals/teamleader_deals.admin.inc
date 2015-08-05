<?php
/**
 * @file
 * Teamleader Deals administration pages.
 */

/**
 * Teamleader Deals settings form.
 */
function teamleader_deals_admin_api_settings($form, &$form_state) {
  $teamleader_users = teamleader_users_get_all_users();
  $client_options = array();
  if (is_array($teamleader_users)) {
    array_walk($teamleader_users, function ($user) use (&$client_options) {
      $client_options[$user['id']] = $user['name'];
    });
  }

  $form[TEAMLEADER_DEALS_DEFAULT_RESPONSIBLE_CLIENT] = array(
    '#type' => 'select',
    '#title' => t('Default responsible client'),
    '#options' => $client_options,
    '#default_value' => variable_get(TEAMLEADER_DEALS_DEFAULT_RESPONSIBLE_CLIENT, 0),
    '#description' => t('You can create new users on Teamleader <a href="@url" target="_blank">here</a>.', array('@url' => url('https://www.teamleader.be/config.php', array('external' => TRUE)))),
    '#required' => TRUE,
  );

  $teamleader_departments = teamleader_general_get_all_departments();
  $department_options = array();

  if (is_array($teamleader_departments)) {
    array_walk($teamleader_departments, function ($department) use (&$department_options) {
      $department_options[$department['id']] = $department['name'];
    });
  }

  $form[TEAMLEADER_DEALS_DEFAULT_DEPARTMENT] = array(
    '#type' => 'select',
    '#title' => t('Default department'),
    '#options' => $department_options,
    '#default_value' => variable_get(TEAMLEADER_DEALS_DEFAULT_DEPARTMENT, 0),
    '#description' => t('You can create new departments on Teamleader <a href="@url" target="_blank">here</a>.', array('@url' => url('https://www.teamleader.be/settings.php', array('external' => TRUE)))),
    '#required' => TRUE,
  );
  $form[TEAMLEADER_DEALS_DEFAULT_SOURCE] = array(
    '#type' => 'textfield',
    '#title' => t('Source'),
    '#default_value' => variable_get(TEAMLEADER_DEALS_DEFAULT_SOURCE, 'Drupal'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
