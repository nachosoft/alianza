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
  $items['cajas/corte'] = array(
    'title' => 'Corte de Caja',
    'page callback' => 'unit_paying_corte_caja', 
    'access arguments' => array('usar caja'), 
    'type' => MENU_CALLBACK, 
  );
  $items['cajas/corte/crear'] = array(
    'title' => 'Corte de Caja',
    'page callback' => 'unit_paying_corte_caja_crear', 
    'access arguments' => array('usar caja'), 
    'type' => MENU_CALLBACK, 
  );

  $items['admin/structure/cajas/reset'] = array(
    'title' => 'Reiniciar cajas',
    'page callback' => 'unit_paying_reset_caja', 
    'access arguments' => array('reiniciar cajas'), 
    'type' => MENU_CALLBACK, 
  );

  $items['cajas/crear/poliza/%'] = array(
    'title' => 'Poliza',
    'page callback' => 'unit_paying_crear_poliza', 
    'access arguments' => array('usar caja'), 
    'page arguments' => array(3),
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
    // el id de la unidad
    $data['nid'] = $_GET['nid'];
  }else{
    return;
  }

  if (isset($_GET['concepts'])) {
    $data['concepts'] = json_decode($_GET['concepts']);
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
  }
  
  if (isset($_GET['caja'])) {
    $data['caja'] = $_GET['caja'];
  }else{
    return;
  }

  $recibo = unit_paying_create_node($data, 'recibo',$user);

  unit_paying_process_precorte($_GET['cc'], $recibo);


  print($recibo);
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

// Funcion para  crear  un corte de caja 
function unit_paying_corte_caja(){
  print_r('hola'); exit; 
  if (isset($_GET['recibos'])) {
    $recibos = $_GET['recibos'];
  } else {
    return false;
  }

  foreach ($recibos as $indice => $recibo) {
    print_r($recibo); exit;
  }
}

// Crear nuevo contenido de corte de caja
function unit_paying_corte_caja_crear(){
  if (isset($_GET['caja'])) {
    $caja = $_GET['caja'];
  } else {
    return false;
  }

  // Crear el nodo
  GLOBAL $user;
  $node = unit_paying_create_node(array('caja'=>$caja),'corte',$user);
  
  //$node = 12324;
  print(json_encode($node));

}

// Reiniciar las cajas 
function unit_paying_reset_caja(){
  $form = drupal_get_form('unit_paying_form');
  return $form;
} 


// Preprocesar el corte de caja
function unit_paying_process_precorte($cid, $nid){
  if (!isset($cid) && !isset($nid)) {
    return false;
  }

  $node = node_load($cid);

  $node->field_recibos[LANGUAGE_NONE][]['target_id'] = $nid;

  node_save($node);

  return;
}

// Generador de poliza
function unit_paying_crear_poliza($nid){
  if (!isset($nid) && $nid == 0) {
    return ;
  }
  $node = node_load($nid);

  $vars = array(
    'no_economico' => NULL, 
    'nombre' => NULL, 
    'marca' => NULL, 
    'placas' => NULL,
    'modelo' => NULL,
    'motor' => NULL,
    'serie' => NULL,
    'fecha_vencimiento' => NULL,
    'fecha_creacion' => NULL, 
  );

  $output = theme('unit_paying_poliza',$vars );
  print_r($output); exit;

  
}

