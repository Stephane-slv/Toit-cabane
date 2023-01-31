function onButton() {
    var xhttp = new XMLHttpRequest();           // https://www.w3schools.com/xml/ajax_intro.asp
    xhttp.open("GET", "on", true);
    xhttp.send();
}

function offButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "off", true);
    xhttp.send();
}

setInterval(function getData()                          // Requette toutes les 500ms
{
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)      // When readyState is 4 and status is 200, the response is ready
        {
            document.getElementById("captToitA_Haut").innerHTML = this.responseText;
        }
    };

    xhttp.open("GET", "lireCapteur", true);
    xhttp.send();
}, 500);

setInterval(function getData()                              // Requette toutes les 5s
{
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)      // When readyState is 4 and status is 200, the response is ready
        {
            document.getElementById("temperature").innerHTML = this.responseText;
        }
    };

    xhttp.open("GET", "lireTemperature", true);
    xhttp.send();
}, 5000);
