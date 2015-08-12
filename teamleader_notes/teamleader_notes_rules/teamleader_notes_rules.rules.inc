<?php

/**
 * @file
 * Contains Rules integration for Teamleader Notes Rules module.
 */

/**
 * Implements hook_rules_action_info().
 */
function teamleader_notes_rules_rules_action_info() {
  $info = array();

  $info['teamleader_notes_rules_rules_action_add_note'] = array(
    'group' => t('Teamleader Notes Rules'),
    'label' => t('Add a note'),
    'named parameter' => TRUE,
    'parameter' => array(
      'object_type' => array(
        'type' => 'text',
        'label' => t('Attach to'),
        'options list' => 'teamleader_notes_rules_rules_action_add_a_note_object_type_options',
      ),
      'object_id' => array(
        'type' => 'integer',
        'label' => t('Object ID'),
      ),
      'title' => array(
        'type' => 'text',
        'label' => t('Title'),
      ),
      'description' => array(
        'type' => 'text',
        'label' => t('Description'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'description_type' => array(
        'type' => 'text',
        'label' => t('Description type'),
        'description' => t('Type of the description text.'),
        'optional' => TRUE,
        'options list' => 'teamleader_notes_rules_rules_action_add_a_note_note_description_type_options',
      ),
      'read_only' => array(
        'type' => 'integer',
        'label' => t('Read only'),
        'optional' => TRUE,
        'options list' => 'teamleader_notes_rules_rules_action_add_a_note_note_read_only_options',
      ),
    ),
  );
  return $info;
}

/**
 * Custom options callback for teamleader_notes_rules_rules_action_add_a_note.
 *
 * @see teamleader_notes_rules_rules_action_info()
 *
 * @return array
 *   Array of options for Customer type field.
 */
function teamleader_notes_rules_rules_action_add_a_note_object_type_options() {
  return array(
    TEAMLEADER_NOTES_ATTACH_TO_CONTACT => TEAMLEADER_NOTES_ATTACH_TO_CONTACT,
    TEAMLEADER_NOTES_ATTACH_TO_COMPANY => TEAMLEADER_NOTES_ATTACH_TO_COMPANY,
    TEAMLEADER_NOTES_ATTACH_TO_MEETING => TEAMLEADER_NOTES_ATTACH_TO_MEETING,
    TEAMLEADER_NOTES_ATTACH_TO_PROJECT => TEAMLEADER_NOTES_ATTACH_TO_PROJECT,
    TEAMLEADER_NOTES_ATTACH_TO_SALE => TEAMLEADER_NOTES_ATTACH_TO_SALE,
  );
}

/**
 * Custom options callback for teamleader_notes_rules_rules_action_add_a_note.
 *
 * @see teamleader_notes_rules_rules_action_info()
 *
 * @return array
 *   Array of options for Customer type field.
 */
function teamleader_notes_rules_rules_action_add_a_note_note_description_type_options() {
  return array(
    'plaintext' => 'plain text',
    'HTML' => 'HTML',
  );
}

/**
 * Custom options callback for teamleader_notes_rules_rules_action_add_a_note.
 *
 * @see teamleader_notes_rules_rules_action_info()
 *
 * @return array
 *   Array of options for Customer type field.
 */
function teamleader_notes_rules_rules_action_add_a_note_note_read_only_options() {
  return array(
    t('No'),
    t('Yes'),
  );
}

/**
 * Rules action which attach a new note to an existing "object" on Teamleader.
 *
 * @param array $parameters
 *   - ... @see teamleader_notes_rules_rules_action_info().
 *   - settings: array
 *       Rules settings array.
 *   - state: RulesState
 *      Rules state object.
 */
function teamleader_notes_rules_rules_action_add_note(array $parameters) {
  drupal_alter('teamleader_crm_rules_rules_action_add_note_data', $parameters);

  $object_type = $title = '';
  $object_id = 0;
  $additional_info = array();

  array_walk($parameters, function ($value, $key) use (&$object_type, &$object_id, &$title, &$additional_info) {
    switch ($key) {
      case 'object_type':
        $object_type = $value;
        break;

      case 'object_id':
        $object_id = $value;
        break;

      case 'title':
        $title = $value;
        break;

      default:
        // Ignore Rules's additional parameters.
        if (!in_array($key, array('settings', 'state'))) {
          $additional_info[$key] = $value;
        }
    }
  });

  if (!teamleader_notes_add_note($object_type, $object_id, $title, $additional_info)) {
    drupal_set_message(t('Something went wrong! Note can not be created on Teamleader.'), 'error');
    return;
  }
}
