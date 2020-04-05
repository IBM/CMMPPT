 $(function() {
    function processform(e) {
   var payload={}
        $('.form-group',$(e.target).parent()).each(function(itag,tag) {
            var input=$("input",tag)
             var value = null
             var fieldname = input.attr('data-fieldname')
             if(input.hasClass("form-check-input")) {
                 value = input.prop("checked")
             }
             else {
                 value=input.val()
             }
             payload[fieldname]=value
             return;
         }
                                                   )
        return payload
    }
     function post(e, verb, action) {
         var payload=processform(e)
         $.post("/visor/"+verb+"?action="+action, payload).done(function(data) {
             $("#status").text(data.sql)
             $("#stdout").text(data.stdout)
             $("#stderr").text(data.stderr)
             return;
         }).fail(function(data) {
             return;
         })
         
     }
     $("#addprinter").click(function(e) {
         // alert('submit pressed')
         post(e,"printer","add")
         e.preventDefault()
     })
     $("#updateprinter").click(function(e) {
         // alert('submit pressed')
         var payload=processform(e)
         $.post("/visor/printer?action=update", payload).done(function(data) {
             $("#status").text(data.sql)
             $("#stdout").text(data.stdout)
             $("#stderr").text(data.stderr)
             return;
         }).fail(function(data) {
             return;
         })
         e.preventDefault()
     })
     $("#deleteprinter").click(function(e) {
         // alert('submit pressed')
         var payload=processform(e)
         $.post("/visor/printer?action=delete", payload).done(function(data) {
             $("#status").text(data.sql)
             $("#stdout").text(data.stdout)
             $("#stderr").text(data.stderr)
             return;
         }).fail(function(data) {
             return;
         })
         e.preventDefault()
     })
     $("#submitmaterial").click(function(e) {
         alert('submit pressed')
         e.preventDefault()
     })
     $("#submitrequest").click(function(e) {
         alert('submit pressed')
         e.preventDefault()
     })
     $("#submitsql").click(function(e) {
         // alert('submit pressed')
         var payload=processform(e)
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
     $('.nav-tabs a').on('shown.bs.tab', function(event){
         var x = $(event.target).text();         // active tab
         var y = $(event.relatedTarget).text();  // previous tab
         $("#status").text("Changing tabs")
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
 }
  )
