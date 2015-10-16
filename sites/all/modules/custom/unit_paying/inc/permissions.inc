<?php
/**
 * Implements hook_permission().
 */
function unit_paying_permission() {
  return array(
    'usar caja' =>  array(
      'title' => t('Usar Caja'),
      'description' => t('Utilizar la funcion de caja'),
    ),
  );
}