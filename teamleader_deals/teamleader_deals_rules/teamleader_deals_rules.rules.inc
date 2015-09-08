<?php

/**
 * @file
 * Contains Rules integration for Teamleader Deals Rules module.
 */

/**
 * Implements hook_rules_action_info().
 *
 * TODO Add product (item lines) and custom fields support.
 */
function teamleader_deals_rules_rules_action_info() {
  $info = array();

  $info['teamleader_deals_rules_rules_action_add_a_deal'] = array(
    'group' => t('Teamleader Deals Rules'),
    'label' => t('Add a deal'),
    'named parameter' => TRUE,
    'parameter' => array(
      'deal_contact_or_company' => array(
        'type' => 'text',
        'label' => t('Deal: Customer type'),
        'options list' => 'teamleader_deals_rules_rules_action_add_a_deal_customer_type_options',
      ),
      'contact_forename' => array(
        'type' => 'text',
        'label' => t('Contact: Forename'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'contact_surname' => array(
        'type' => 'text',
        'label' => t('Contact: Surname'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'contact_email' => array(
        'type' => 'text',
        'label' => t('Contact: Email'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'company_name' => array(
        'type' => 'text',
        'label' => t('Company: Name'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'deal_title' => array(
        'type' => 'text',
        'label' => t('Deal: Title'),
      ),
      'deal_description' => array(
        'type' => 'text',
        'label' => t('Deal: Description'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'deal_description_location' => array(
        'type' => 'text',
        'label' => t('Deal: Save description to'),
        'optional' => TRUE,
        'options list' => 'teamleader_deals_rules_rules_action_add_a_deal_deal_description_location_options',
        'default value' => 'deal',
        'description' => t("If you create a %action rules action, which will be triggered by the %event event, then you save the description field value to a new, attached note, instead of the deal's own description field. <p><strong>Warning: </strong> if you chose the 'New note' option, and you do/will not have this particular action implemented, then you'll lost the value of the description field.</p>", array(
          '%action' => t('Teamleader Notes Rules: Add a note'),
          '%event' => t('Teamleader Deals Rule: New deal added'),
        )),
      ),
      'deal_source' => array(
        'type' => 'text',
        'label' => t('Deal: Source'),
        'default value' => variable_get(TEAMLEADER_DEALS_DEFAULT_SOURCE, 'Drupal'),
      ),
      'deal_sys_department_id' => array(
        'type' => 'integer',
        'label' => t('Deal: Department (ID)'),
        'default value' => variable_get(TEAMLEADER_DEALS_DEFAULT_DEPARTMENT, 0),
        'options list' => 'teamleader_deals_rules_rules_action_add_a_deal_department_options',
      ),
      'deal_responsible_sys_client_id' => array(
        'type' => 'text',
        'label' => t('Deal: Responsible client (ID)'),
        'default value' => variable_get(TEAMLEADER_DEALS_DEFAULT_RESPONSIBLE_CLIENT, 0),
        'options list' => 'teamleader_deals_rules_rules_action_add_a_deal_responsible_options',
      ),
    ),
  );
  return $info;
}

/**
 * Custom options callback for teamleader_deals_rules_rules_action_add_a_deal.
 *
 * @see teamleader_deals_rules_rules_action_info()
 *
 * @return array
 *   Array of options for "Deal: Customer type" field.
 */
function teamleader_deals_rules_rules_action_add_a_deal_customer_type_options() {
  return array(
    TEAMLEADER_DEALS_CUSTOMER_TYPE_CONTACT => TEAMLEADER_DEALS_CUSTOMER_TYPE_CONTACT,
    TEAMLEADER_DEALS_CUSTOMER_TYPE_COMPANY => TEAMLEADER_DEALS_CUSTOMER_TYPE_COMPANY,
  );
}

/**
 * Custom options callback for teamleader_deals_rules_rules_action_add_a_deal.
 *
 * @see teamleader_deals_rules_rules_action_info()
 *
 * @return array
 *   Array of options for "Deal: Department" field.
 */
function teamleader_deals_rules_rules_action_add_a_deal_department_options() {
  $teamleader_departments = teamleader_general_get_all_departments();
  $department_options = array();
  array_walk($teamleader_departments, function ($department) use (&$department_options) {
    $department_options[$department['id']] = $department['name'];
  });

  return $department_options;
}

/**
 * Custom options callback for teamleader_deals_rules_rules_action_add_a_deal.
 *
 * @see teamleader_deals_rules_rules_action_info()
 *
 * @return array
 *   Array of options for "Deal: Responsible client" field.
 */
function teamleader_deals_rules_rules_action_add_a_deal_responsible_options() {
  $teamleader_users = teamleader_users_get_all_users();
  $client_options = array();
  array_walk($teamleader_users, function ($user) use (&$client_options) {
    $client_options[$user['id']] = $user['name'];
  });

  return $client_options;
}

/**
 * Custom options callback for teamleader_deals_rules_rules_action_add_a_deal.
 *
 * @see teamleader_deals_rules_rules_action_info()
 *
 * @return array
 *   Array of options for "Deal: Save description to" field.
 */
function teamleader_deals_rules_rules_action_add_a_deal_deal_description_location_options() {
  return array(
    'deal' => t("Deal's description"),
    'note' => t('New note'),
  );
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function teamleader_deals_rules_form_rules_ui_add_element_alter(&$form, &$form_state, $form_id) {
  _teamleader_deals_rules_form_rules_ui_add_edit_form_alter($form, $form_state);
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function teamleader_deals_rules_form_rules_ui_edit_element_alter(&$form, &$form_state, $form_id) {
  _teamleader_deals_rules_form_rules_ui_add_edit_form_alter($form, $form_state);
}

/**
 * Custom form alter function for Rules UI add/edit forms.
 */
function _teamleader_deals_rules_form_rules_ui_add_edit_form_alter(&$form, &$form_state) {
  if (isset($form_state['rules_element'])) {
    $rules_action = $form_state['rules_element'];
    if (is_a($rules_action, 'RulesAction') && $rules_action->getElementName() == 'teamleader_deals_rules_rules_action_add_a_deal') {
      // Hide/Display fields according to the value of the
      // deal_contact_or_company parameter.
      foreach (element_children($form['parameter']) as $element) {
        if (preg_match('/^contact_/', $element)) {
          $form['parameter'][$element]['#states'] = array(
            'visible' => array(
              ':input[name="parameter[deal_contact_or_company][settings][deal_contact_or_company]"]' => array('value' => TEAMLEADER_DEALS_CUSTOMER_TYPE_CONTACT),
            ),
          );

          if ($element == 'contact_email') {
            $form['parameter'][$element]['#states']['required'] = array(
              ':input[name="parameter[deal_contact_or_company][settings][deal_contact_or_company]"]' => array('value' => TEAMLEADER_DEALS_CUSTOMER_TYPE_CONTACT),
            );
          }
        }
        if (preg_match('/^company_/', $element)) {
          $form['parameter'][$element]['#states'] = array(
            'visible' => array(
              ':input[name="parameter[deal_contact_or_company][settings][deal_contact_or_company]"]' => array('value' => TEAMLEADER_DEALS_CUSTOMER_TYPE_COMPANY),
            ),
            'required' => array(
              ':input[name="parameter[deal_contact_or_company][settings][deal_contact_or_company]"]' => array('value' => TEAMLEADER_DEALS_CUSTOMER_TYPE_COMPANY),
            ),
          );
        }
      }
      $form['parameter']['deal_description_location']['#access'] = module_exists('teamleader_notes_rules');
      $form['#validate'][] = 'teamleader_deals_rules_teamleader_deals_rules_rules_action_add_a_deal_form_validate';
    }
  }
}

/**
 * Rules action which add a participant to an existing event in Teamleader.
 *
 * @param array $parameters
 *   - deal_contact_or_company: text
 *       Could be one from TEAMLEADER_DEALS_CUSTOMER_TYPE_CONTACT or
 *       TEAMLEADER_DEALS_CUSTOMER_TYPE_COMPANY.
 *   - contact_forename: text
 *       Forename of the contact.
 *   - ... @see teamleader_deals_rules_rules_action_info().
 *   - settings: array
 *       Rules settings array.
 *   - state: RulesState
 *      Rules state object.
 */
function teamleader_deals_rules_rules_action_add_a_deal(array $parameters) {
  $contact_or_company_id = 0;

  drupal_alter('teamleader_deals_rules_rules_action_add_a_deal_data', $parameters);

  switch ($parameters['deal_contact_or_company']) {
    case TEAMLEADER_DEALS_CUSTOMER_TYPE_CONTACT:
      if (empty($parameters['contact_email'])) {
        drupal_set_message(t("Contact's email can not be empty, if you would like to add a deal to the Teamleader with contact customer type!"), 'error');
        return;
      }

      $contact = teamleader_crm_search_contacts($parameters['contact_email']);

      // The isn't any matching contact in the Teamleader, we have to create
      // this one first.
      if (!is_array($contact)) {
        if (empty($parameters['contact_forename']) || empty($parameters['contact_surname'])) {
          drupal_set_message(t("Contact does not exists on Teamleader. New contact can not be created, because the contact's forename and surname are empty, that's why the deal creation process is also aborted!"), 'error');
          return;
        }

        $forename = $surname = $email = '';
        $additional_info = array();

        array_walk($parameters, function ($value, $key) use (&$forename, &$surname, &$email, &$additional_info) {
          switch ($key) {
            case 'contact_forename':
              $forename = $value;
              break;

            case 'contact_surname':
              $surname = $value;
              break;

            case 'contact_email':
              $email = $value;
              break;

            default:
              if (preg_match('/^contact_/', $key)) {
                $clean_key = preg_replace('/^contact_/', '', $key);
                $additional_info[$clean_key] = $value;
              }
          }
        });

        $contact = teamleader_crm_add_contact($forename, $surname, $email, $additional_info);

        if (!$contact) {
          drupal_set_message(t("New contact can not be created on Teamleader, that's why the deal can not be added!"), 'error');
          return;
        }
      }
      // Only the first item will be considered as the "real" existing contact
      // from the results.
      else {
        $contact = array_shift($contact);
        $changes = array();
        array_walk($parameters, function ($value, $key) use (&$changes) {
          if (preg_match('/^contact_/', $key)) {
            $clean_key = preg_replace('/^contact_/', '', $key);
            $changes[$clean_key] = $value;
          }
        });
        if (!teamleader_crm_update_contact($contact['id'], $changes)) {
          drupal_set_message(t('Existing contact can not be updated on Teamleader!'), 'warning');
        }
      }

      $contact_or_company_id = is_array($contact) ? $contact['id'] : $contact;

      break;

    case TEAMLEADER_DEALS_CUSTOMER_TYPE_COMPANY:
      if (empty($parameters['company_name'])) {
        drupal_set_message(t("Company's name can not be empty, if you would like to add a deal to the Teamleader with company customer type!"), 'error');
        return;
      }

      $company = teamleader_crm_search_companies($parameters['company_name']);

      // There isn't any matching company in Teamleader, we have to create this
      // one first.
      if (!is_array($company)) {
        $name = '';
        $additional_info = array();

        array_walk($parameters, function ($value, $key) use (&$name, &$additional_info) {
          switch ($key) {
            case 'company_name':
              $name = $value;
              break;

            default:
              if (preg_match('/^company_/', $key)) {
                $clean_key = preg_replace('/^company_/', '', $key);
                $additional_info[$clean_key] = $value;
              }
          }
        });

        $company = teamleader_crm_add_company($name, $additional_info);

        if (!$company) {
          drupal_set_message(t("New company can not be created on Teamleader, that's why the deal can not be added!"), 'error');
          return;
        }
      }
      // Only the first item will be considered as the "real" existing company
      // from the results.
      else {
        $company = array_shift($company);
      }

      $contact_or_company_id = is_array($company) ? $company['id'] : $company;
      break;
  }

  $additional_info = array(
    'description' => $parameters['deal_description'],
    'description_location' => $parameters['deal_description_location'],
  );

  if (!teamleader_deals_add_deal($parameters['deal_contact_or_company'], $contact_or_company_id, $parameters['deal_title'], $parameters['deal_sys_department_id'], $parameters['deal_responsible_sys_client_id'], $parameters['deal_source'], $additional_info, array(), array())) {
    drupal_set_message(t('Something went wrong! Deal can not be created on Teamleader.'), 'error');
  }
}

/**
 * Implements hook_rules_event_info().
 *
 * TODO Extend available variables.
 */
function teamleader_deals_rules_rules_event_info() {
  return array(
    'teamleader_deals_rules_rules_event_new_deal_added' => array(
      'label' => t('New deal added'),
      'group' => t('Teamleader Deals Rules'),
      'variables' => array(
        'deal_id' => array(
          'type' => 'integer',
          'label' => t('Deal ID'),
          'description' => t('The ID of the new deal on the Teamleader.'),
        ),
        'description' => array(
          'type' => 'text',
          'label' => t('Description'),
          'description' => t('The description of the deal.'),
        ),
      ),
    ),
  );
}
