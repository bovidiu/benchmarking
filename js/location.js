(function($){

    $("#selectServerLocation").live('click',function(event){
        event.preventDefault();
        $.ajax({
            dataType: "json",
            url: "/benchmarking/location",
            success: function(result){
                if(result.statusCode != 200){
                    return alert(result.statusText);
                }

                $.each(result.data,function(i){
                    $("#loadLocation").append('<a href="#" class="server" id="'+i+'" >'+i + '-' + this.PendingTests.Total+'</a><br/>');
                });

            }
        });
    });

    $(".server").live('click',function(event){
        event.preventDefault();
        $("#edit-location").val(this.id);
        $("#loadLocation").slideToggle('slow');
    });

})(jQuery)
