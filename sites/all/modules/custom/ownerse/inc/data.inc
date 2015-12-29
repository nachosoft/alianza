<?php

// Sacar los usuarios del csv y ordenarlos
function ownerse_get_owners_csv(){
  ini_set('auto_detect_line_endings', TRUE);

  $csv_path = drupal_get_path('module','ownerse').'/data/duenos.csv';

  $rows = array_map('str_getcsv', file($csv_path));
  $header = array_shift($rows);
  $csv = array(
    
  );

  foreach ($rows as $row) {
    $csv[] = array_combine($header, $row);
  }

  $result = array();
  foreach ($csv as $data) {
    $socio = $data['socio'];
    if ($socio != 'VACIO' && $socio != 'FORANEO' && !empty($socio)) {
      $result[$socio][] = $data['no'];
    }
  }
  return $result;

}

// Crea el nodo de dueño y le asigna las unidades correspondientes
function ownerse_export($owner,$units){
  $nid = ownerse_create_node($owner,$units);
  ownerse_register_units($nid,$units);
  //sleep(1);
  return $nid;
}

/** 
 * Creador de nuevos nodos
 * @Params $owner -> string, $units = array;
 * @Return $node
 **/
function ownerse_create_node($owner, $units){
  if (isset($owner) && isset($units)) { 
    global $user;

    $node = new stdClass();  // Create a new node object
    $node->title = $owner;
    $node->type = 'owner';  // Content type
    $node->language = LANGUAGE_NONE;  // Or e.g. 'en' if locale is enabled
    $node->revision = 1; // set revision
    node_object_prepare($node);  //Set some default values
    $node->uid = $user->uid;    
    
    // Default node settings.
    $node->status = 1;   // (1 or 0): published or unpublished
    $node->promote = 0;  // (1 or 0): promoted to front page or not
    $node->sticky = 0;  // (1 or 0): sticky at top of lists or not
    $node->comment = 0;  // 2 = comments open, 1 = comments closed, 0 = comments hidden

    // Unidades
    foreach ($units as $key => $unit) {
      $nid = ownerse_get_unit_node($unit);
      if ($nid) {
        $node->field_unit[$node->language][]['target_id'] = $nid;
      } else {
        $node->field_importacion_error[$node->language][0]['value'] = 1;
      }
    }
    
    node_submit($node);
    node_save($node);

    return $node->nid;

  } else {
    return 'error,  no hay suficiente informacion :(';
  }
}


// Tener la unidad del nodo
function ownerse_get_unit_node($unit){
  if (!isset($unit)) {
    return FALSE;
  }

  // Select User.
  $sql = sprintf("SELECT entity_id FROM {field_data_field_numero_economico} WHERE bundle = 'unit' AND field_numero_economico_value = %d", $unit);
  
  $results = db_query($sql);  

  $row = $results->fetchAssoc();

  if (isset($row['entity_id'])) {
    return $row['entity_id'];
  }else{
    return false;
  }
}

// Registrar el owner para cada unidad 
function ownerse_register_units($nid,$units){
  if (isset($units)) {
    foreach ($units as $key => $unit) {
      $unit_nid = ownerse_get_unit_node($unit);
      if ($unit_nid) {
        $unit = node_load($unit_nid);
        $unit->field_owner[LANGUAGE_NONE][0]['target_id'] = $nid;

        node_save($unit);
      }
    }
  }
}
