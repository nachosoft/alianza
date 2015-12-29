(function ($) {
  Drupal.behaviors.alianzaMenu = {
    attach: function (context, settings) {
      $('#block-system-main-menu', context).once('MainMenu', function () {
        $('#block-system-main-menu h2.block-title').click(menuToggle());
      });
    }
  };
  function menuToggle(){
    var parent = $(this).parent();
    if(parent.hasClass('active')){
      parent.slideToggle(slow);
      parent.removeClass('active');
    } else {
      parent.slideToggle(slow);
      parent.addClass('active');
    }
  }
})(jQuery);