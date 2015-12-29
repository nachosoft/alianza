<?php

/**
 * Implements hook_form().
 */
function unit_paying_form($node, &$form_state) {
  //$type = node_type_get_type($node);

  $form['select_caja'] = array(
    '#type' => 'select',
    '#title' => t('Selecciona la caja'),
    '#options' => array(
      '__NULL' => 'Seleccionar',
      1 => 'Club de Servicios',
      2 => 'Tesoreria'
    ),
    '#description' => t('Selecciona la caja que sera reiniciada, esta accion no se puede deshacer!'),
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Reiniciar'),
  );

  return $form;
}

// Reiniciar la caja 
function unit_paying_form_submit($form_id, &$form_state){
  if ($form_state['values']['select_caja'] == '__NULL') {
    form_set_error('select_caja', t('Seleccione alguna caja'));
    return false;
  }

  $cid = $form_state['values']['select_caja'];

  $insert_sql = "UPDATE {unit_paying_caja_settings} SET folio_caja = 0 WHERE caja = %d ";
  $insert_sql = sprintf($insert_sql, 
    $cid 
  );
  $insert_result = db_query($insert_sql);

  drupal_set_message(t('La caja ha sido Reiniciada'), 'status', FALSE);
  return;
}


