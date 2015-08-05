<?php

/**
 * @file
 * Contains Rules integration for Teamleader Deals Rules module.
 */

/**
 * Implements hook_rules_action_info().
 */
function teamleader_crm_rules_rules_action_info() {
  $info = array();

  $info['teamleader_crm_rules_rules_action_add_contact'] = array(
    'group' => t('Teamleader CRM Rules'),
    'label' => t('Add contact'),
    'named parameter' => TRUE,
    'parameter' => array(
      'forename' => array(
        'type' => 'text',
        'label' => t('Contact: Forename'),
      ),
      'surname' => array(
        'type' => 'text',
        'label' => t('Contact: Surname'),
      ),
      'email' => array(
        'type' => 'text',
        'label' => t('Contact: Email'),
      ),
      'telephone' => array(
        'type' => 'text',
        'label' => t('Contact: Phone number'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'street' => array(
        'type' => 'text',
        'label' => t('Contact: Street'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'number' => array(
        'type' => 'text',
        'label' => t('Contact: Street number'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'zipcode' => array(
        'type' => 'text',
        'label' => t('Contact: Zip code'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'city' => array(
        'type' => 'text',
        'label' => t('Contact: City'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'country' => array(
        'type' => 'text',
        'label' => t('Contact: Country'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
    ),
  );
  $info['teamleader_crm_rules_rules_action_update_contact'] = array(
    'group' => t('Teamleader CRM Rules'),
    'label' => t('Update contact'),
    'named parameter' => TRUE,
    'parameter' => array(
      'contact_id' => array(
        'type' => 'integer',
        'label' => t('Contact: Teamleader ID'),
      ),
      'track_changes' => array(
        'type' => 'integer',
        'label' => t('Teamleader: Track changes'),
        'default value' => 1,
        'options list' => 'teamleader_crm_rules_rules_action_update_contact_track_changes_options',
      ),
      'forename' => array(
        'type' => 'text',
        'label' => t('Contact: Forename'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'surname' => array(
        'type' => 'text',
        'label' => t('Contact: Surname'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'email' => array(
        'type' => 'text',
        'label' => t('Contact: Email'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'telephone' => array(
        'type' => 'text',
        'label' => t('Contact: Phone number'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'street' => array(
        'type' => 'text',
        'label' => t('Contact: Street'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'number' => array(
        'type' => 'text',
        'label' => t('Contact: Street number'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'zipcode' => array(
        'type' => 'text',
        'label' => t('Contact: Zip code'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'city' => array(
        'type' => 'text',
        'label' => t('Contact: City'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'country' => array(
        'type' => 'text',
        'label' => t('Contact: Country'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
    ),
  );
  return $info;
}

/**
 * Custom options callback for teamleader_crm_rules_rules_action_update_contact.
 *
 * @see teamleader_crm_rules_rules_action_info()
 *
 * @return array
 *   Array of options for Track changes field.
 */
function teamleader_crm_rules_rules_action_update_contact_track_changes_options() {
  return array(
    t('No'),
    t('Yes'),
  );
}

/**
 * Rules action which creates a contact on the Teamleader.
 *
 * @param array $parameters
 *   - forename : text
 *       The first name of the person.
 *   - surname : text
 *       The last name of the person.
 *   - ... @see teamleader_crm_rules_rules_action_info().
 *   - settings: array
 *       Rules settings array.
 *   - state: RulesState
 *      Rules state object.
 */
function teamleader_crm_rules_rules_action_add_contact(array $parameters) {
  drupal_alter('teamleader_crm_rules_rules_action_add_contact_data', $parameters);

  $contact = teamleader_crm_search_contacts($parameters['email']);

  // Contact does not exists on the Teamleader.
  if (!is_array($contact)) {
    $forename = $surname = $email = '';
    $additional_info = array();

    array_walk($parameters, function ($value, $key) use (&$forename, &$surname, &$email, &$additional_info) {
      switch ($key) {
        case 'forename':
          $forename = $value;
          break;

        case 'surname':
          $surname = $value;
          break;

        case 'email':
          $email = $value;
          break;

        default:
          // Ignore Rules's additional parameters.
          if (!in_array($key, array('settings', 'state'))) {
            $additional_info[$key] = $value;
          }
      }
    });

    $contact = teamleader_crm_add_contact($forename, $surname, $email, $additional_info);

    if (!$contact) {
      drupal_set_message(t('Something went wrong! Contact can not be created on Teamleader.'), 'error');
      return;
    }
  }
}

/**
 * Rules action which updates an existing contact on the Teamleader.
 *
 * @param array $parameters
 *   - contact_id: int
 *       Teamleader contact ID of the person.
 *   - forename : text
 *       The first name of the person.
 *   - surname : text
 *       The last name of the person.
 *   - ... @see teamleader_crm_rules_rules_action_info().
 *   - settings: array
 *       Rules settings array.
 *   - state: RulesState
 *      Rules state object.
 */
function teamleader_crm_rules_rules_action_update_contact(array $parameters) {
  drupal_alter('teamleader_crm_rules_rules_action_update_contact_data', $parameters);

  $contact = teamleader_crm_get_contact_info($parameters['contact_id']);

  // Contact is exists on the Teamleader.
  if ($contact) {
    $contact_id = $track_changes = 0;
    $additional_info = array();

    array_walk($parameters, function ($value, $key) use (&$contact_id, &$track_changes, &$additional_info) {
      switch ($key) {
        case 'contact_id':
          $contact_id = $value;
          break;

        case 'track_changes':
          $track_changes = $value;
          break;

        default:
          // Ignore Rules's additional parameters.
          if (!in_array($key, array('settings', 'state'))) {
            $additional_info[$key] = $value;
          }
      }
    });

    if (!teamleader_crm_update_contact($contact_id, $additional_info, $track_changes)) {
      drupal_set_message(t('Something went wrong! Contact can not be updated on Teamleader.'), 'error');
      return;
    }
  }
}
