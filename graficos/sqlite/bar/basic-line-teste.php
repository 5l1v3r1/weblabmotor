<?php
/**
 *
 *
 * @link          https://github.com/mzm-dev 
 * @demo          http://highcharts-mzm.rhcloud.com
 * @created       September 2014
 * @fb            https://www.facebook.com/zakimedia
 * @email         mohdzaki04@gmail.com
 */
$cakeDescription = "Highcharts Pie Chart";
?>

<!DOCTYPE HTML>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
        <title><?php echo $cakeDescription ?></title>
        <link href="../webroot/css/cake.generic.css" type="text/css" rel="stylesheet">
        <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.7.1/jquery.min.js"></script>
        <script src="http://code.highcharts.com/highcharts.js"></script>
        <script src="http://code.highcharts.com/stock/highstock.js"></script>
        <script src="http://code.highcharts.com/modules/exporting.js"></script>
        <script type="text/javascript">

        
        var chart;

            $(document).ready(function() {
                var options = {
                    chart: {
                        renderTo: 'container',
                        type: 'line'
                    },
                    
                    title: {
                        text: 'Gráfico de temperaturas',
                        x: -20 //center
                    },
                    subtitle: {
                        text: 'Temperaturas do motor',
                        x: -20
                    },
                    exporting: {
                        enabled: true
                    },
                    rangeSelector: {
                        buttons: [{
                            count: 1,
                            type: 'minute',
                            text: '1M'
                        }, {
                            count: 5,
                            type: 'minute',
                            text: '5M'
                        }, {
                            type: 'all',
                            text: 'All'
                        }],
                        inputEnabled: true,
                        selected: 1
                    },
                    xAxis: {
                        categories: [],
                        title: {
                            text: 'Tempo'
                        }
                    },
                    yAxis: {
                        title: {
                            text: 'Temperatura'
                        },
                        plotLines: [{
                                value: 0,
                                width: 1,
                                color: '#808080'
                            }]
                    },
                    tooltip: {
                        valueSuffix: '°C'
                    },
                    legend: {
                        layout: 'vertical',
                        align: 'right',
                        verticalAlign: 'middle',
                        borderWidth: 0
                    },
                    series: []
                };
                var url =  "http://localhost/graficos/sqlite/bar/data/data-basic-colm-teste.php";
                $.getJSON(url,  function(json) {
                    options.xAxis.categories = json[0]['data']; //xAxis: {categories: []}
                    options.series[0] = json[1];
                    options.series[1] = json[2];
                    chart = new Highcharts.Chart(options);
                });

                // The button action
                $('#button').click(function() {
                    var url =  "http://localhost/graficos/sqlite/bar/data/data-basic-colm-teste.php";
                    $.getJSON(url,  function(json) {
                        options.xAxis.categories = json[0]['data']; //xAxis: {categories: []}
                        options.series[0] = json[1];
                        options.series[1] = json[2];
                        chart = new Highcharts.Chart(options);
                    });
                });

            });

        </script>
        <button id="button">Atualizar dados...</button>
    </head>
    <body>
        <a class="link_header" href="/mysql/">&lt;&lt; Back to index</a>
        <div id="container" style="min-width: 400px; height: 400px; margin: 0 auto"></div>
    </body>
</html>