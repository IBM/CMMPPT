$(function() {
    $.get('/visor/authenticate').done(function(data) {
        if(data.status) {
            $(location).attr('href', data.url)
        }
        function processform(e) {
            var payload={}
            $('.form-group',$(e.target).parent()).each(function(itag,tag) {
                var inputs=$("input",tag)
                inputs.each(function(itag,tag){
                    var input = $(tag)
                    var value = null
                    var fieldname = input.attr('data-fieldname')
                    var keyfield=input.attr('data-key')?(input.attr('data-key')=="true"):false
                    if(input.attr("type")=="checkbox") {
                        value = input.prop("checked")
                        payload[fieldname]={value: value, keyfield:keyfield}
                    }
                    else if(input.attr("type")=="radio") {
                        if(input.prop("checked")) {
                            fieldname=input.attr("name")
                            value=input.attr('data-value')
                            payload[fieldname]={value: value, keyfield:keyfield}
                        }
                    }
                    else {
                        value=input.val()
                        payload[fieldname]={value: value, keyfield:keyfield}
                    }
                })
                return
            })
            return payload
        }
        function post(e, verb, action) {
            var payload=processform(e)
            payload={json: JSON.stringify(payload)}
            $.post("/visor/"+verb+"?action="+action, payload).done(function(data) {
                $("#status").text(data.sql)
                $("#stdout").text(data.stdout)
                $("#stderr").text(data.stderr)
                return;
            }).fail(function(data) {
                return;
            })
            
        }
        function hasrole(role) {
            return data.roles.includes(role)
        }
        if (hasrole("administrator")) {
            $("#sql").css("display","")
            $("#sqltab").css("display","")
        }
        if (hasrole("administrator")||hasrole("producer")) {
            $("#printerpanel").css("display","")
            $("#printertab").css("display","")
            $("#materialpanel").css("display","")
            $("#materialtab").css("display","")
        }
        if (hasrole("administrator") ||  hasrole("consumer")) {
            $("#quantitypanel").css("display","")
            $("#quantitytab").css("display","")
        }
        $("#addprinter").click(function(e) {
            // alert('submit pressed')
            post(e,"printer","add")
            e.preventDefault()
        })
        $("#updateprinter").click(function(e) {
            // alert('submit
            post(e,"printer","update")
            e.preventDefault()
        })
        $("#deleteprinter").click(function(e) {
            // alert('submit pressed')
            post(e,"printer","delete")
            e.preventDefault()
        })
        $("#addmaterial").click(function(e) {
            // alert('submit pressed')
            post(e,"material","add")
            e.preventDefault()
        })
        $("#updatematerial").click(function(e) {
            // alert('submit
            post(e,"material","update")
            e.preventDefault()
        })
        $("#deletematerial").click(function(e) {
            // alert('submit pressed')
            post(e,"material","delete")
            e.preventDefault()
        })
        $("#addrequest").click(function(e) {
            // alert('submit pressed')
            post(e,"request","add")
            e.preventDefault()
        })
        $("#updaterequest").click(function(e) {
            // alert('submit
            post(e,"request","update")
            e.preventDefault()
        })
        $("#deleterequest").click(function(e) {
            // alert('submit pressed')
            post(e,"request","delete")
            e.preventDefault()
        })
        $("#submitsql").click(function(e) {
            // alert('submit pressed')
            var payload=processform(e)
            payload={json: JSON.stringify(payload)}
            $.post("/visor/sql", payload).done(function(data) {
                $("#status").text(data.sql)
                $("#stdout").text(data.stdout)
                $("#stderr").text(data.stderr)
                return;
            }).fail(function(data) {
                return;
            })
            e.preventDefault()
        })
        $("#logout").click(function() {
            $.get("/visor/logout").done(function(data) {
                $(location).attr("href",data.url)
            })
        })
        $('.nav-tabs a').on('shown.bs.tab', function(event){
            var x = $(event.target).text();         // active tab
            var y = $(event.relatedTarget).text();  // previous tab
            $("#status").text(`Changing tabs: ${y} => ${x}`)
            $("#stdout").text("")
            $("#stderr").text("")
        });
        /*
          var forms = document.getElementsByClassName('needs-validation');
          // Loop over them and prevent submission
          var validation = Array.prototype.filter.call(forms, function(form) {
          form.addEventListener('submit', function(event) {
          if (form.checkValidity() === false) {
          event.preventDefault();
          event.stopPropagation();
          }
          form.classList.add('was-validated');
          }, false);
          });
        */
    })
}
 )
