$ ->
  @keys = ["ip", "start_led", "end_led", "red", "green", "blue"]
  _.each @keys, (key) ->
    if value = $.cookie(key)
      $("##{key}").val(value)
  
  $("#set").on "click", =>
    k = {}
    _.each @keys, (key) ->
      value = $("##{key}").val()
      k[key] = value
      $.cookie(key, value)
    
    $.get "http://#{k.ip}:8080/?command=#{k.start_led},#{k.end_led},#{k.red},#{k.green},#{k.blue}"
    
    false
