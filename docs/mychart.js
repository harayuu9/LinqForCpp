var data = [];

function loadJson(json, tag)
{
    json.benchmarks.map(benchmark => {
        if(benchmark.name.startsWith("Native"))
        {
            var name = benchmark.name.replace("Native", "");
            const find = data.find(d => d.name == name);
            if(find == undefined)
            {
                var pushData = {name: name, data:[]};
                pushData.data.push({tag: tag, nativeTime: benchmark.real_time});
                data.push(pushData);
            }
            else
            {
                const find2 = find.data.find(d => d.tag == tag);
                if(find2 == undefined)
                {
                    find.data.push({tag: tag, nativeTime: benchmark.real_time});
                }
                else
                {
                    find2.nativeTime = benchmark.real_time;
                }
            }
        }
        else if(benchmark.name.startsWith("MyLinq"))
        {
            var name = benchmark.name.replace("MyLinq", "");
            const find = data.find(d => d.name == name);
            if(find == undefined)
            {
                var pushData = {name: name, data:[]};
                pushData.data.push({tag: tag, myLinqTime: benchmark.real_time});
                data.push(pushData);
            }
            else
            {
                const find2 = find.data.find(d => d.tag == tag);
                if(find2 == undefined)
                {
                    find.data.push({tag: tag, myLinqTime: benchmark.real_time});
                }
                else
                {
                    find2.myLinqTime = benchmark.real_time;
                }
            }
        }
        else if(benchmark.name.startsWith("CppLinq"))
        {
            var name = benchmark.name.replace("CppLinq", "");
            const find = data.find(d => d.name == name);
            if(find == undefined)
            {
                var pushData = {name: name, data:[]};
                pushData.data.push({tag: tag, cppLinqTime: benchmark.real_time});
                data.push(pushData);
            }
            else
            {
                const find2 = find.data.find(d => d.tag == tag);
                if(find2 == undefined)
                {
                    find.data.push({tag: tag, cppLinqTime: benchmark.real_time});
                }
                else
                {
                    find2.cppLinqTime = benchmark.real_time;
                }
            }
        }
    });
}

function drawImpl()
{
    var numId = 0;
    data.map(rowData => {
        const labels = [];
        var myLinqData = [];
        var nativeData = [];
        var cppLinqData = [];
        rowData.data.map(oneData =>{
            labels.push(oneData.tag + "/" + rowData.name);
            myLinqData.push(oneData.myLinqTime);
            nativeData.push(oneData.nativeTime);
            cppLinqData.push(oneData.cppLinqTime);
        });

        var id = "mychart" + numId;
        document.writeln('<div style="width: 60%;">');
        document.writeln(`<canvas id="${id}"></canvas>`);

        const ctx = document.getElementById(id).getContext("2d");
        const myChart = new Chart(ctx, {
            type: "bar",
            data: {
                labels: labels,
                datasets: [
                    {label: "MyLinq", data: myLinqData, backgroundColor: "Red"},
                    {label: "Native", data: nativeData, backgroundColor: "LightGreen"},
                    {label: "CppLinq", data: cppLinqData, backgroundColor: "SkyBlue"}
                ]
            }
        });
        numId++;
        document.writeln('</div>');
    });
}

function drawBarChart(filePath, tag, init = false,end = false) {
    var req = new XMLHttpRequest();
    req.open("GET", filePath, true);
    req.onload = function () {
        if(init)
        {
            var head = document.head;
            var linkElement = document.createElement("link");
            linkElement.type = "text/css";
            linkElement.rel = "stylesheet";
            linkElement.href = "style.css";
            head.appendChild(linkElement);
        }

        const json = JSON.parse(req.responseText);
        loadJson(json, tag);

        if(end)
        {
            drawImpl();
        }
    };
    req.send(null);
}

function main() {
    drawBarChart("Json/windows-latest.CPP17.json", "Win17", true);
    drawBarChart("Json/windows-latest.CPP20.json", "Win20");
    drawBarChart("Json/ubuntu-latest.CPP17.json", "Ubuntu17");
    drawBarChart("Json/ubuntu-latest.CPP20.json","Ubuntu20");
    drawBarChart("Json/macos-latest.CPP17.json", "Mac17");
    drawBarChart("Json/macos-latest.CPP20.json", "Mac20", false, true);
}

main();