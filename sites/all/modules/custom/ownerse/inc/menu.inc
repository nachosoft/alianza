<?php
/**
 * Implements hook_menu().
 */
function ownerse_menu() {
  /*
   *$items['blog'] = array(
   *  'title' => 'blogs',
   *  'page callback' => 'blog_page',
   *  'access arguments' => array('access content'),
   *  'type' => MENU_SUGGESTED_ITEM,
   *);
   */
  $items['admin/owners'] = array(
    'title' => 'Exportar Usuarios',
    'page callback' => 'ownerse_menu_export',
    'access arguments' => array('usar caja'), 
    'type' => MENU_CALLBACK,
  );

  return $items;
}


// Funcion para exportar usuarios
function ownerse_menu_export(){

  $owners_units = ownerse_get_owners_csv();
  $exported_nodes = array();
  foreach ($owners_units as $key => $owner) {
    $nid = ownerse_export($key, $owner);
    $exported_nodes[] = $nid;
  }  

  print_r($exported_nodes); 

}