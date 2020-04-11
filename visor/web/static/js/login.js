$(function() {
    $("#login").click(function(e) {
    e.preventDefault();
        target=$(e.target).parent()
        psw= $("input[name='psw']", target)
        uname= $("input[name='uname']", target)
        data={psw:psw.val(),uname:uname.val()}
        $.post("/visor/login", data,
              ).done(function(data) {
              if(data.status) {
              $("#status").text(data.message)
              }
              else {
                  $(location).attr("href",data.url)
                  }
              }).fail(function(data){
                console.log(data)
              }).always(function(data){
                console.log(data)
              })
    })
})
