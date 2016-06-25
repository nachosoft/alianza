<?php
/**
 * Implements hook_theme().
 */
function unit_paying_theme($existing, $type, $theme, $path) {
  return array(
    'unit_paying_caja_club' => array(
      'template' => 'theme/unit_paying_caja_club', 
      'variables' => array(
        'selector' => NULL, 
        'info' => NULL, 
        'recibo' => NULL, 
      ),
    ),
    'unit_paying_poliza' => array(
      'template' => 'theme/unit_paying_poliza', 
      'variables' => array(
        'no_economico' => NULL, 
        'nombre' => NULL, 
        'marca' => NULL, 
        'placas' => NULL,
        'modelo' => NULL,
        'motor' => NULL,
        'serie' => NULL,
        'fecha_vencimiento' => NULL,
        'fecha_creacion' => NULL, 
      ),
    ),
  );
}