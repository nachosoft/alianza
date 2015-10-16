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
  );
}