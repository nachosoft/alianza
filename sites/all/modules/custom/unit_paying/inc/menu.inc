<?php
/**
 * Implements hook_menu().
 */
function unit_paying_menu() {
  $items['caja'] = array(
    'page callback' => 'unit_paying_menu_club', 
    'access arguments' => array('usar caja'), 
    'type' => MENU_CALLBACK, 
  );
  $items['cajas/process'] = array(
    'title' => 'Caja procesar',
    'page callback' => 'unit_paying_process', 
    'access arguments' => array('usar caja'), 
    'type' => MENU_CALLBACK, 
  );
  $items['codes/unic'] = array(
    'title' => 'Unico concepto Dueño',
    'page callback' => 'unit_paying_code_validate', 
    'access arguments' => array('usar caja'), 
    'type' => MENU_CALLBACK, 
  );

  return $items;
}

// Retorna pagina de cobros de caja de servicios.
function unit_paying_menu_club(){

  $vars = array(
    'info' => NULL, 
    'recibo' => NULL, 
  );

  drupal_add_js(drupal_get_path('module', 'unit_paying') . '/js/angular.min.js');
  drupal_add_js(drupal_get_path('module', 'unit_paying') . '/js/caja.paying.js');

  $output = theme('unit_paying_caja_club',$vars );

  //print_r($output); exit;
  return $output;
}

// Procesar pago, y creacion de recibo.
function unit_paying_process(){
  $data = array(); 

  GLOBAL $user;

  // Revisar los ingredientes necesarios.
  if (isset($_GET['nid'])) {
    $data['nid'] = $_GET['nid'];
  }else{
    return;
  }

  if (isset($_GET['concepts'])) {
    $data['concepts'] = $_GET['concepts'];
  }else{
    return;
  }

  if (isset($_GET['payment'])) {
    $data['payment'] = $_GET['payment'];
  }else{
    return;
  }

  if (isset($_GET['debt'])) {
    $data['debt'] = $_GET['debt'];
  }else{
    return;
  }
  
  if (isset($_GET['caja'])) {
    $data['caja'] = $_GET['caja'];
  }else{
    return;
  }

  $recibo = unit_paying_create_node($data, 'recibo',$user);

  print_r($data);
}


// Revisa si el codigo es unico para ese dueno
function unit_paying_code_validate(){
  if (isset($_GET['unit'])) {
    $unitId = $_GET['unit'];
  } else {
    return false;
  }

  if (isset($_GET['id'])) {
    $codeId = $_GET['id'];
  } else {
    return false;
  }


  $unit = node_load($unitId);

  $owner = node_load($unit->field_owner[LANGUAGE_NONE][0]['target_id']);
  $response =  0;

  if (is_array($owner->field_unic_codes[LANGUAGE_NONE])) {
    foreach ($owner->field_unic_codes[LANGUAGE_NONE] as $key => $code) {
      if (in_array($codeId, $code)) {
        $response = 1;
      }
    }
  }


  echo $response;
  //print_r($response);
}
