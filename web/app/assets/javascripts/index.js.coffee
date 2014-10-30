$ ->
  $("#set").on "click", ->
    ip = $("#ip").val()
    start_led  = $("#start_led").val()
    end_led = $("#end_led").val()
    red = $("#red").val()
    green = $("#green").val()
    blue = $("#blue").val()
    
    $.get "http://#{ip}:8080/?command=#{start_led},#{end_led},#{red},#{green},#{blue}"
    
    false
