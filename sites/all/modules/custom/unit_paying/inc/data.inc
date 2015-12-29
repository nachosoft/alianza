<?php
/** 
 * Creador de nuevos nodos
 * @Params $data -> array(), $type = string;
 * @Return $node
 **/
function unit_paying_create_node($data, $type, $user){

  if (isset($data) && isset($type)) { 
    $node = new stdClass();  // Create a new node object
    $node->title = 'title';
    $node->type = $type;  // Content type
    $node->language = LANGUAGE_NONE;  // Or e.g. 'en' if locale is enabled
    $node->revision = 1; // set revision
    node_object_prepare($node);  //Set some default values
    $node->uid = $user->uid;    
    
    // Default node settings.
    $node->status = 1;   // (1 or 0): published or unpublished
    $node->promote = 0;  // (1 or 0): promoted to front page or not
    $node->sticky = 0;  // (1 or 0): sticky at top of lists or not
    $node->comment = 1;  // 2 = comments open, 1 = comments closed, 0 = comments hidden
    
    // Specific node settings.
    switch ($type) {
      case 'recibo':
        $node->title = unit_paying_get_caja_number($data['caja']);
        $node->field_unidad[$node->language][0]['target_id'] = $data['nid'];
        $concepts = $data['concepts'];
        if (isset($data['debt'])) {
          $node->field_deuda[$node->language][0]['value'] = $data['debt'];
        }
        $node->field_pago[$node->language][0]['value'] = $data['payment'];

        if (isset($data['debt'])) {
          $new_debt = intval() - intval($data['payment']);
          if ($new_debt < 0) {
            $new_debt = 0;
          }
          $node->field_total[$node->language][0]['value'] = $new_debt;
        }else{
          $node->field_total[$node->language][0]['value'] = 0;
        }
        

        break;

      case 'corte':
        $t = time();

        $node->title = date("d_m_Y",$t);
        $node->field_tipo_de_caja[$node->language][0]['value'] = $data['caja'];

        break;

      default:
        # code...
        break;
    }

    node_submit($node);

    node_save($node);



    // create the field collection. 
    if ($type == 'recibo') {
      foreach ($concepts as $key => $concept) {
        unit_paying_create_concepts_fc($node->nid,$concept);
      }
    }
    

    return $node->nid;

  } else {
    return 'error,  no hay suficiente informacion :(';
  }
}

// Consult the number of the caja
function unit_paying_get_caja_number($cid){
  $folio_caja = array(
    1 => 'club-',
    2 => 'tes-', 
  );
  $folio = null;
  $sql = sprintf("SELECT folio_caja FROM unit_paying_caja_settings WHERE caja = %d" , $cid);
  $results = db_query($sql)->fetchAll();
  if (isset($results[0]->folio_caja)) {

    $nfolio = $results[0]->folio_caja + 1;
    //save new folio 
    $insert_sql = "UPDATE {unit_paying_caja_settings} SET folio_caja = %d WHERE caja = %d ";
    $insert_sql = sprintf($insert_sql, 
      $nfolio, 
      $cid 
    );
    $insert_result = db_query($insert_sql);

    $folio = $folio_caja[$cid] . $nfolio;
  } else {
    $c_request = db_insert('unit_paying_caja_settings')
      ->fields(array(
        'caja' => $cid,
        'folio_caja' => 1,
      ))
      ->execute();
    $folio = $folio_caja[$cid] . 1; 
  }
   
  return $folio;
}

// Crear el field collection de los conceptos
function unit_paying_create_concepts_fc($nid,$concept){
  if (!isset($nid)) {
    return false;
  }

  // Load target node
  $node = node_load($nid);

  // Create a new field collection 
  $fieldCollectionItem = entity_create('field_collection_item', array('field_name' => 'field_conceptos_fields'));


  // Load items into field collection
  $fieldCollectionItem->field_conceptos[LANGUAGE_NONE][]['target_id'] = $concept->id;

  $fieldCollectionItem->field_valor[LANGUAGE_NONE][]['value'] = $concept->valor; 

  // Save field collection item
  $fieldCollectionItem->setHostEntity('node', $node);

  $fieldCollectionItem->save(TRUE); 

  node_save($node);

  return true;
 
}

