var cajaApp = angular.module('cajaApp', []);
var $j = jQuery.noConflict();

jQuery(document).ready(function($){
  // Asignar nombre a la caja
  if (localStorage["caja"]) {
    caja = localStorage["caja"];
    cajaText = 'Caja';
    if(caja == 1){
      cajaText = 'Caja - Club de Servicios';
    } else if(caja == 2){
      cajaText = 'Caja - Tesoreria';
    }
    $('.page-header').text(cajaText);
  };
  // Crear selector de caja
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

  // tomar el submit de el selector de caja
  $('#get_caja').submit(function(event) {
    cajaValue = $('#caja').val();
    var d = new Date();
    var n = Date.parse(d);

    // Id de la caja
    localStorage.setItem('caja', cajaValue);
    // Fecha de apertura
    localStorage.setItem('cajaOpen', n);

    // Crear corte de caja
    var request = $.ajax({
      url: "/cajas/corte/crear",
      method: "GET",
      data: { caja : cajaValue },
      dataType: "html"
    });
     
    request.done(function( msg ) {
      localStorage.setItem('cajaCorteNid', msg);
      console.log('Nodo creado - '+ msg);
    });

    location.reload();
    return false;
  });

});

cajaApp.controller('searchCtrl', ['$scope', '$http', function ($scope,$http) {
  $scope.noEconomico = '';
  $scope.deuda = 0;
  $scope.placas = '';
  $scope.ownerId = 0;
  $scope.ownerName = '';
  $scope.unitId = 0;
  $scope.pagar = false;

  $scope.deudaStatus = 1;
  // Hace la suma de los conceptos y la deuda remanente de la unidad
  $scope.calcularDeuda = function(){
    var total = 0;
    for(var i = 0; i < $scope.conceptos.length; i++){
      var concept = $scope.conceptos[i];
      total += parseInt(concept.valor);
    }

    if ($scope.deudaStatus != 0) {
      total += parseInt($scope.deuda);
    };
    

    return total ;
  };

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

  $scope.calcularCambio = function(){
    dif = $scope.deudaRecibo - $scope.pago;
    if (dif < 0 ){
      dif = dif *-1;
      dif = Math.round(dif * 100) / 100;
      dif = 'Cambio $' + dif; 
    }else{
      dif = 'no dar cambio';
    }
    return dif;
  }

  // Optiene el valor para los conceptos de valor asignable
  $scope.getValor = function(label){
    var value = prompt("Inserta el valor para "+label, 0);
    if (value != null) {
      return value;
    }
  }

  // Revisa si el cobro ha sido unico para ese dueno.
  $scope.getUniqueValor = function(unidad,id,valorDefault){
    
    
  }

  //Busca el numero de unidad y retorna los datos de la unidad y del dueno
  $scope.buscarNo = function(){
    if ($scope.n_unidad) {

      $http.get('/rest/units?no='+$scope.n_unidad).
        success(function(data, status, headers, config) {
          // this callback will be called asynchronously
          // when the response is available
          //console.log(data);
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

  // Busca el codigo de cobro
  $scope.buscarCodigo = function(){
    if ($scope.unitId == 0) {
      alert('Seleccione una unidad');
      return;
    };
    if ($scope.codigo) {
      $http.get('/rest/codigos?code='+$scope.codigo+'&caja='+$scope.caja).
        success(function(data, status, headers, config) {
          // this callback will be called asynchronously
          // when the response is available
          if (data[0]){
            // Si el valor es asignable abre el popup
            if(data[0].tipo == 2){
              data[0].Valor = $scope.getValor(data[0].Concepto);
            }
            // si el valor es unico por dueño calcula a 0 si ya lo pago una vez
            if (data[0].unico == 1) {
              //var valorUnico = $scope.getUniqueValor($scope.unitId, data[0].nid,data[0].Valor);
              var Valor = data[0].Valor;
             
              $j.get( "/codes/unic", { unit: $scope.unitId, id: data[0].nid } )
                .done(function( dat ) {
                  console.log(dat);
                  if (parseInt(dat) == 0) {
                    alert('Este Dueño ya pago este concepto, al ser unico, el costo se volvera $0.00');
                    Valor = 0;
                    console.log(dat);
                    data[0].Valor = Valor;
                  };
                });
            };
            cData = {
              titulo  : data[0].Codigo +' '+ data[0].Concepto,
              valor   : data[0].Valor, 
              periodo : data[0].Periodo, 
              id      : data[0].nid, 
            };
            $scope.conceptos.push(cData);
            $scope.deudaRecibo = $scope.calcularDeuda();
            $scope.codigo_message = 'Codigo de Concepto';
            console.dir($scope.conceptos);
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

  // Remueve conceptos y recalcula la deuda
  $scope.removeConcept = function(index){
    $scope.conceptos.splice(index, 1);
    $scope.deudaRecibo = $scope.calcularDeuda();
  };

  

  // Si hay algun cobro por hacer, abrira el popup de pago.
  $scope.abrirPago = function(){
    if ($scope.unitId == 0) {
      alert('Seleccione una unidad');
      return;
    };
    console.log($scope.pagar);
    $scope.paraPagar = true;
  };

  $scope.cerrarPago = function(){
    $scope.paraPagar = false;
  };

  // Manda la llamada de ajax para crear el recibo, y abre el recibo en una ventana emergente
  $scope.terminarPago = function(){
    var obj = {},
    arr = $scope.conceptos;
    l = arr.length; 

    while( l && (obj[--l] = arr.pop() ) ){};
    //console.log(JSON.stringify(obj));
    console.log('/cajas/process?nid='+$scope.unitId+'&concepts='+JSON.stringify(obj)+'&caja='+localStorage["caja"]+'&payment='+$scope.pago+'&cc='+localStorage["cajaCorteNid"]);
    $http.get('/cajas/process?nid='+$scope.unitId+'&concepts='+JSON.stringify(obj)+'&caja='+localStorage["caja"]+'&payment='+$scope.pago+'&cc='+localStorage["cajaCorteNid"]).
      success(function(data, status, headers, config) {
        console.log($scope.pagar);

        if ($scope.pagar) {
          var win = window.open('/node/'+parseInt(data), '_blank');
          win.focus();
        }else{
          var win = window.open('/print/'+parseInt(data), '_blank');
          win.focus();
        };
        
      }).
      error(function(data, status, headers, config) {
        // called asynchronously if an error occurs
        // or server returns response with an error status.
        console.log(data);
        console.log(status);
      });
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

cajaApp.filter('currencySimbol',
  function () {
    return function(input) {
      return '$'+ input;
    }
});
