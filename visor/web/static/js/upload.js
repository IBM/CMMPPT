$(function() {
    $("#fileUpload").change(function(e) {
        formdata=new FormData()
        files=this.files
        for(var f=0;f<files.length;++f) {
            file=files[f]
            formdata.append(file.name,file)
        }
        $.ajax({url:'/visor/senddata',
                type:'POST',
                data:formdata,
                cache:false,
                contentType:false,
                processData: false
               }).done(function(data){
                   return;
               }).fail(function(data) {
                   return;
               })
        e.preventDefault()
        return;
    })
})
