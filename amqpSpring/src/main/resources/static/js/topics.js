console.log("start");

var temptopicname;

$(function() {
    $.get("http://localhost:8080/topics", function(topics) {
        var topicList = $("#topics ul");
        var previousItem = null; // 保存之前被点击的 <li> 元素

        for (var i = 0; i < topics.length; i++) {
            var topicName = topics[i];
            var item = $("<li>").text(topicName);

            // 添加点击事件处理程序
            item.click(function() {
                if (previousItem !== null) {
                    previousItem.removeClass("active");
                }

                $(this).addClass("active");
                previousItem = $(this);

                var topicName = $(this).text();
                console.log("111");
                temptopicname = topicName;
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
                var timestamp = messages[i].timestamp; // 假设timestamp是数据库中存储的时间戳
                // 创建一个新的Date对象
                var date = new Date(timestamp);
                // 增加八个小时
                date.setHours(date.getHours() + 8);
                // 获取增加后的时间部分（不包括毫秒）
                var formattedTime = date.toISOString().replace(/[.].*/, "");
                // 创建列表项
                var item = $("<li>").text("time : " + formattedTime.replace("T"," ") + "-------content : " + messages[i].content);

                //var item = $("<li>").text("time : " + messages[i].timestamp.replace(/[.].*/, "") + "-------content : " + messages[i].content);
                item.addClass("message")
                contents.append(item);
            }
        },
        error: function () {
          alert("search erron!");
        }
    });
}


var countdownIntervalId;

function startCountdown() {
    clearInterval(countdownIntervalId); // 清除之前的定时器

    var interval = parseInt(document.getElementById("intervalInput").value);
    if (!isNaN(interval) && interval > 0) {
        var countdownText = document.getElementById("countdownText");
        countdownText.textContent = "该部分将在 " + interval + " 秒后进行刷新";

        var countdown = interval;
        countdownIntervalId = setInterval(function() {
            countdown--;
            countdownText.textContent = "该部分将在 " + countdown + " 秒后进行刷新";

            if (countdown <= 0) {
                countdown = interval;
                loadContent(temptopicname);
            }
        }, 1000);
    }
}

document.getElementById("startButton").addEventListener("click", startCountdown);

