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
    'reiniciar cajas' =>  array(
      'title' => t('Reiniciar Cajas'),
      'description' => t('Reiniciar los folios de las Cajas'),
    ),
  );
}