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
        $concepts = explode(',', $data['concepts']);
        foreach ($concepts as $key => $concept) {
          $node->field_conceptos[$node->language][]['target_id'] = $concept;
        }
        $node->field_deuda[$node->language][0]['value'] = $data['debt'];
        $node->field_pago[$node->language][0]['value'] = $data['payment'];

        $new_debt = intval($data['debt']) - intval($data['payment']);
        if ($new_debt < 0) {
          $new_debt = 0;
        }
        $node->field_total[$node->language][0]['value'] = $new_debt;
        break;

      default:
        # code...
        break;
    }


    node_submit($node);

    node_save($node);

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
  $results = db_query($sql)->fetchField();

  if (isset($results)) {
    $nfolio = $results + 1;

    //save new folio 
    $insert_sql = "UPDATE {unit_paying_caja_settings} SET folio_caja = %d WHERE caja = %d ";
    $insert_sql = sprintf($insert_sql, 
      $nfolio, 
      $cid 
    );
    $insert_result = db_query($insert_sql);

    $folio = $folio_caja[$cid] . $nfolio;
  } else {
    return false;
    /*$c_request = db_insert('unit_paying_caja_settings')
      ->fields(array(
        'caja' => $cid,
        'folio_caja' => 1,
      ))
      ->execute();
    $folio = $folio_caja[$cid] . 1; */
  }
   
  return $folio;
}

