function processform(formtag) {
    var payload={}
    $(formtag).each(function(itag,tag) {
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
function setstatus(data) {
                $("#status").text(data.sql)
                $("#stdout").text(data.stdout)
                $("#stderr").text(data.stderr)
}
$(function() {
    $.get('/visor/authenticate').done(function(data) {
        if(data.status) {
            $(location).attr('href', data.url)
        }
        function processform_(e) {
            return processform($('.form-group',$(e.target).parent()))
        }
        function post(e, verb, action) {
            var payload=processform_(e)
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
            var payload=processform_(e)
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
        $('#downloadtable').click(function(e) {
            var tablename=null;
            $("input[type='radio']",$(e.target).parent().parent()).each(function(itag,tag){
                if($(tag).prop("checked")) {
                    tablename=$(tag).attr("data-value")
                }
            })
            $("#table").empty()
            $("#table").append($("<div>").attr("width", "100%").css("text-align","center").
            append($("<span>").addClass("spinner-border spinner-border-sm").
            attr("role", "status").attr("aria-hidden", "true").css("width","200px").css("height","200px").css("color","red")))
            $.get("/visor/download", {table:tablename}).done(function(data) {
                $("#table").empty()
                location=data.url
            })
            })
        $('#submittable').click(function(e) {
            var tablename=null;
            $("input[type='radio']",$(e.target).parent().parent()).each(function(itag,tag){
                if($(tag).prop("checked")) {
                    tablename=$(tag).attr("data-value")
                }
            })
            $("#table").empty()
            $("#table").append($("<div>").attr("width", "100%").css("text-align","center").
            append($("<span>").addClass("spinner-border spinner-border-sm").
            attr("role", "status").attr("aria-hidden", "true").css("width","200px").css("height","200px").css("color","red")))
            $.get("/visor/table", {table:tablename}).done(function(data) {
                $("#table").empty()
                var columns=JSON.parse(data["columns"])
                var rows=JSON.parse(data["rows"])
                var tablediv = $("#table")
                var table = $("<table>")
                tablediv.append(table)
                table.addClass("table table-striped table-bordered table-sm").attr("cellspacing","0").attr("width","100%")
                var thead = $("<thead>")
                table.append(thead)
                var th=$("<tr>")
                thead.append(th)
                var c=[]
                columns.forEach(function(column) {
                    var name=column.column_name
                    c.push(name)
                    var td=$("<th>").addClass("th-sm").text(name)
                    th.append(td)
                })
                var tbody=$("<tbody>")
                table.append(tbody)
                rows.forEach(function(row) {
                    var tr=$("<tr>")
                    tbody.append(tr)
                    for(cc in c) {
                        var name=row[c[cc]]
                        var td=$("<td>").text(name)
                        tr.append(td)
                    }
                })
                        $(table).DataTable({
            "scrollY": "200px",
            "scrollCollapse": true,
            });
                    $('.dataTables_length').addClass('bs-select');

                return
            })
            return;
        })
        $('.nav-tabs a').on('shown.bs.tab', function(event){
            var x = $(event.target).text();         // active tab
            var y = $(event.relatedTarget).text();  // previous tab
            $("#status").text(`Changing tabs: ${y} => ${x}`)
            $("#stdout").text("")
            $("#stderr").text("")
        });
        $.get("/visor/tables").done(function(data) {
            tables=JSON.parse(data.stdout)
            var table=$("<table>")
            $("#selecttable").append(table)
            table.css("width","100%")
            var tr = $("<tr>")
            table.append(tr)
            for(var i in tables) {
                var table=tables[i].table_name
                var id = table+"_radio"
                var td=$("<td>")
                tr.append(td)
                td.append(
                    $("<input>").attr("type","radio").attr("id",id).
                    attr("name","table_radio").
                    attr("data-value",table).
                    addClass("form-check-input")
                )
                td.append($("<label>").addClass("form-check-label").attr("for",id).text(table))
            }
        })
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
