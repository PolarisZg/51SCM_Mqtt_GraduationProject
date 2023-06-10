console.log("start");

var temptopicname;
var nowPageNum;
var nowtopicName;

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
                loadContent(topicName,0,11);
                nowPageNum = 0;
                nowtopicName = topicName;

                var top = document.getElementById("topbutton");
                var buttom = document.getElementById("buttombutton");
                // 清空div内的所有元素
                top.innerHTML = '';
                buttom.innerText = '';
                // 创建四个按钮元素
                var button1 = document.createElement("button");
                var button2 = document.createElement("button");
                var button3 = document.createElement("button");
                var button4 = document.createElement("button");
                // 添加按钮文本
                button1.innerText = "上一页";
                button2.innerText = "下一页";
                button3.innerText = "上一页";
                button4.innerText = "下一页";

                // 绑定按钮点击事件的空函数
                button1.onclick = loadPrePageContent;
                button2.onclick = loadNextPageContent;
                button3.onclick = loadPrePageContent;
                button4.onclick = loadNextPageContent;
                top.appendChild(button1);
                top.appendChild(button2);
                buttom.appendChild(button3);
                buttom.appendChild(button4);

                (document.getElementById("nowPageNum")).textContent="当前页码为 " + nowPageNum ;
            });

            topicList.append(item);
        }
    });
});


function loadContent(topicName,pageNum,pageLength){
    pageNum = parseInt(pageNum,10);
    pageLength = parseInt(pageLength,10);
    console.log(pageLength)
    $.ajax({
        url: "http://localhost:8080/message",
        type: "GET",
        data:{
            topic: topicName,
            page_num: pageNum,
            page_length: pageLength
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
                loadContent(temptopicname,0,11);
            }
        }, 1000);
    }
}

document.getElementById("startButton").addEventListener("click", startCountdown);

function loadNextPageContent() {
    nowPageNum++;
    loadContent(nowtopicName,nowPageNum,11);
    (document.getElementById("nowPageNum")).textContent="当前页码为 " + nowPageNum ;
}

function loadPrePageContent() {
    if(nowPageNum > 0){
        nowPageNum--;
        loadContent(nowtopicName,nowPageNum,11);
        (document.getElementById("nowPageNum")).textContent="当前页码为 " + nowPageNum ;
    }
}
