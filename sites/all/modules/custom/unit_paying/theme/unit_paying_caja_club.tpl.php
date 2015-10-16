<div id="caja" class="club" ng-app="cajaApp">
  <div class="caja_inner" ng-controller="searchCtrl">
    <div class="selector_form">
      <form id="selector" ng-submit="buscarNo()" style="display:inline-block">
        <input type="text" ng-model="n_unidad" id="n_unidad" name="n_unidad" placeholder="No. de Unidad">
        <label>
          <input class="btn btn-info btn-lg" type="submit" value="buscar">
        </label>
      </form>
      <div style="margin-left:20px; display:inline-block;">
        <div class="btn btn-default btn-sm"  >
          <label>
            <input type="checkbox" ng-model="deudaStatus" name="deudaStatus" ng-true-value="0" ng-false-value="1">
            <span>Ocultar deuda de unidad</span>
          </label>
        </div>
        <button ng-click="deudaRecibo = calcularDeuda()" class="btn btn-info btn-lg" >
          <span class="glyphicon glyphicon-refresh"></span> Actualizar
        </button>
      </div>
    </div>
    <div class="info card">
      <div class="socio info_block odd">
        <div class="socio_title ">
          <h2 class="socio_title title">Socio</h2>
        </div>
        <div class="info_item nombre">
          <span>Nombre:</span>
          <p ng-bind="ownerName"></p>
        </div>
      </div>
      <div class="unidad info_block even">
        <h2 class="title">Unidad</h2>
        <div class="info_item no_economico">
          <span>No Economico:</span>
          <p ng-bind="noEconomico"></p>
        </div>
        <div class="info_item deuda">
          <span>Deuda:</span>
          <p>{{deuda | currency}}</p>
        </div>
        <div class="info_item placas">
          <span>Placas:</span>
          <p ng-bind="placas"></p>
        </div>
      </div>
    </div>

    <div class="recibo">
      <div class="cerrar_caja">
        <button ng-click='cierreCaja()' class="btn btn-info btn-lg">Cerrar Caja</button>
      </div>
      <div class="cobrar">
        <button ng-click='abrirPago()' class="btn btn-success btn-lg">Cobrar</button>
      </div>
      <div class="actions">
          <div class="add_concepts">
            <form id="add_form" ng-submit="buscarCodigo()">
              <label for="concept_add" style="display: inline-block;" ng-bind="codigo_message">Codigo de Concepto</label>
              <input type="text" ng-model="codigo" id="concept_add" name="concept_add" placeholder="00-00">
              <input type="submit" class="btn btn-info btn-lg" value="Agregar Concepto">
            </form>
          </div>
        </div>
      <div class="conceptos">
        <table class="table table-striped table-responsive">
          <caption>Conceptos</caption>
          <thead>
            <tr>
              <th>Periodo</th>
              <th>Concepto</th>
              <th>Cargo</th>
              <th>Abono</th>
              <th></th>
            </tr>
          </thead>
          <tbody id="conceptos" ng-repeat="(key, concept) in conceptos">
            <tr class="item">
              <td>{{concept.periodo | htmlToPlaintext}}</td>
              <td>{{concept.titulo}}</td>
              <td>{{concept.valor | currencySimbol}}</td>
              <td>{{concept.valor | currencySimbol}}</td>
              <td><button ng-click='removeConcept(key)' class="btn btn-danger erase">Eliminar</button></td>
            </tr>
          </tbody>
          <tfoot>
            <tr class="danger">
              <td></td>
              <td></td>
              <td>Deuda:</td>
              <td>{{deudaRecibo | currency}}</td>
              <td></td>
            </tr>
            <tr class="success">
              <td></td>
              <td></td>
              <td>Pago:</td>
              <td>{{pago | currency}}</td>
              <td></td>
            </tr>
          </tfoot>
        </table>
      </div>
    </div>
    <div id="forma_pago" ng-show="paraPagar" class="ng-hide">
      <form ng-submit="terminarPago()" class="form-group" id="pago_form">
        <span ng-click="cerrarPago()" class="btn btn-danger" style="float: right;">Cerrar</span> 
        <div class="info">
          <p>No Economico: {{noEconomico}}</p>
          <p>Deuda: {{deudaRecibo | currency}}</p>
        </div>
        <label for="pago">Pago:</label>
        <input type="number" id="pago" name="pago"  ng-model="pago" placeholder="$0.00">
        <div><h3> {{calcularCambio()}}</h3></div>
        <input class="btn btn-success btn-lg" type="submit" value="pagar">
      </form>
    </div>
  </div>
</div>