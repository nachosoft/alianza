var cajaApp = angular.module('cajaApp', []);

jQuery(document).ready(function($){

  if (!localStorage["caja"]) {
    body = $('body');

    form  = '<div class="hard_overlay">';
    form += '<form role="form" id="get_caja">';
    form += '<h2 style="color:#fff;">Seleccionar Caja</h2>';
    form += '<select class="form-control" id="caja" name="caja"><option value="1">Club de Servicios</option><option value="2">Tesoreria</option></select>';
    form += '<input id="submit" class="btn btn-default" type="submit" value="Escojer">';
    form += '</form>';
    form += '</div>';

    body.prepend(form);
  };

  $('#get_caja').submit(function(event) {
    cajaValue = $('#caja').val();

    localStorage.setItem('caja', cajaValue);

    location.reload();
    return true;
  });

});

cajaApp.controller('searchCtrl', ['$scope', '$http', function ($scope,$http) {
  $scope.noEconomico = '';
  $scope.deuda = 0;
  $scope.placas = '';
  $scope.ownerId = 0;
  $scope.ownerName = '';
  $scope.unitId = 0;

  if (localStorage.caja) {
    $scope.caja = localStorage["caja"];
  } else {
    $scope.caja = '';
  }

  $scope.deudaConceptos = 0;
  $scope.deudaRecibo = 0;
  $scope.pago = 0;
  $scope.nuevaDeuda = '$0.00';

  $scope.codigo_message = 'Codigo de Concepto';
  $scope.conceptos = [];

  $scope.paraPagar = null;

  $scope.getValor = function(label){
    var value = prompt("Inserta el valor para "+label, 0);
    if (value != null) {
      return value;
    }
  }

  $scope.buscarNo = function(){
    if ($scope.n_unidad) {

      $http.get('/rest/units?no='+$scope.n_unidad).
        success(function(data, status, headers, config) {
          // this callback will be called asynchronously
          // when the response is available
          if (data[0]){
            $scope.noEconomico = data[0].node_title;
            $scope.deuda = data[0].Deuda;
            $scope.placas = data[0].placas;
            $scope.ownerName = data[0].owner;
            $scope.deudaRecibo = $scope.calcularDeuda();
            $scope.unitId = data[0].nid;
          }
          
        }).
        error(function(data, status, headers, config) {
          // called asynchronously if an error occurs
          // or server returns response with an error status.
          console.log(data);
          console.log(status);
        }); 

    };
  };

  $scope.buscarCodigo = function(){
    if ($scope.codigo) {
      $http.get('/rest/codigos?code='+$scope.codigo+'&caja='+$scope.caja).
        success(function(data, status, headers, config) {
          // this callback will be called asynchronously
          // when the response is available
          if (data[0]){
            if(data[0].tipo == 2){
              data[0].Valor = $scope.getValor(data[0].Concepto);
            }
            cData = {
              titulo  : data[0].Codigo +' '+ data[0].Concepto,
              valor   : data[0].Valor, 
              periodo : data[0].Periodo, 
              id      : data[0].nid, 
            };
            $scope.conceptos.push(cData);
            $scope.deudaRecibo = $scope.calcularDeuda();
            $scope.codigo_message = 'Codigo de Concepto';
          } else {
            $scope.codigo_message = 'Codigo no valido';
          }
          
        }).
        error(function(data, status, headers, config) {
          // called asynchronously if an error occurs
          // or server returns response with an error status.
          console.log(data);
          console.log(status);
        }); 
    };
  };

  $scope.removeConcept = function(index){
    $scope.conceptos.splice(index, 1);
    $scope.deudaRecibo = $scope.calcularDeuda();
  };

  $scope.calcularDeuda = function(){
    var total = 0;
    for(var i = 0; i < $scope.conceptos.length; i++){
        var concept = $scope.conceptos[i];
        total += parseInt(concept.valor);
    }

    total += parseInt($scope.deuda);

    return total ;
  };

  $scope.abrirPago = function(){
    if ($scope.unitId == 0) {
      alert('Seleccione una unidad');
      return;
    };
    $scope.paraPagar = true;
  };

  $scope.cerrarPago = function(){
    $scope.paraPagar = false;
  };

  $scope.terminarPago = function(){
    
    $http.get('/rest/units?no='+$scope.n_unidad).
      success(function(data, status, headers, config) {
          
      }).
      error(function(data, status, headers, config) {
        // called asynchronously if an error occurs
        // or server returns response with an error status.
        console.log(data);
        console.log(status);
      });
    console.log($scope.conceptos);
  }
}]);

cajaApp.filter('htmlToPlaintext',
  function () {
    return function(input) {
      return input.replace(/<[^>]+>/gm, '');
    }
});

cajaApp.filter('currency',
  function () {
    return function(input) {
      return '$'+ input +'.00';
    }
});

  


