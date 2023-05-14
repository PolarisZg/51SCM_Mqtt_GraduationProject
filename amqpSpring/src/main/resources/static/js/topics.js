console.log("start");

$(function (){
    $.get("http://localhost:8080/topics",function (topics){
        var topicList = $("#topics ul");
        for(var i = 0 ; i < topics.length; i++){
            var topicName = topics[i];
            var item = $("<li>").text(topicName);
            item.click(function (){
                var topicName = $(this).text();
                console.log("111");
                loadContent(topicName);
            });
            topicList.append(item);
        }
    });
});

function loadContent(topicName){
    $.ajax({
        url: "http://localhost:8080/message",
        type: "GET",
        data:{
            topic: topicName,
            page_num: 0,
            page_length: 11
        },
        success: function (messages){
            var contents = $("#contents ul");
            contents.empty();
            for(var i = 0 ; i < messages.length; i++){
                var item = $("<li>").text("time : " + messages[i].timestamp.replace(/[.].*/, "") + "-------content : " + messages[i].content);
                item.addClass("message")
                contents.append(item);
            }
        },
        error: function () {
          alert("search erron!");
        }
    });
}
