/// <reference path="jquery.d.ts" />
/// <reference path='jqueryui.d.ts' />

var PieChart = (function () {
    function PieChart(data, element, selectHandler) {
        this.data = data;
        this.element = element;
        this.selectHandler = selectHandler;
        this.chart = new google.visualization.PieChart(this.element);
    }
    PieChart.prototype.drawChart = function () {
        var dataTable = google.visualization.arrayToDataTable(this.data);

        var options = {
            pieHole: 0.4,
            legend: { position: 'none' },
            chartArea: { width: '100%', height: '90%' },
            pieSliceTextStyle: { fontName: "Helvetica" },
            tooltip: { trigger: 'none' },
            pieSliceText: 'label'
        };

        this.chart.draw(dataTable, options);

        google.visualization.events.addListener(this.chart, 'select', this.selectHandler);
    };

    PieChart.prototype.getSelectedIndex = function () {
        return this.chart.getSelection()[0].row;
    };
    return PieChart;
})();

var MainScreen = (function () {
    function MainScreen(data, element, selectHandler) {
        this.data = data;
        this.element = element;
        this.chart = new PieChart(data, document.getElementById('donutchart'), selectHandler);
        this.chart.drawChart();
        clearDataElement();
    }
    return MainScreen;
})();

var TableScreen = (function () {
    function TableScreen(data, title) {
        this.element = document.createElement("div");
        this.element.setAttribute("id", "tablescreen");
        this.element.setAttribute("class", "appscreen");

        var header = document.createElement("div");
        header.setAttribute("id", "header");
        this.element.appendChild(header);

        this.button = document.createElement("button");
        this.button.setAttribute("class", "ios-6-arrow left blue");
        this.button.setAttribute("data-title", "Back");
        this.button.setAttribute("onClick", "setMainScreen()");
        header.appendChild(this.button);

        var titleText = document.createElement("span");
        titleText.setAttribute("id", "tabletitle");
        titleText.appendChild(document.createTextNode(title));
        header.appendChild(titleText);

        this.table = document.createElement("table");
        this.table.setAttribute("id", "expensetable");
        var row = this.addRow('Date', 'Payee', 'Amount');
        for (var i = 0; i < data.length; ++i) {
            var datarow = data[i];
            this.addRow("" + year + "." + (month + 1) + "." + datarow[0], datarow[1], datarow[2] + " €");
        }
        row.setAttribute("class", "header");
        this.element.appendChild(this.table);
    }
    TableScreen.prototype.addRow = function (val1, val2, val3) {
        var row = this.table.insertRow();
        this.AddCell(row, val1);
        this.AddCell(row, val2);
        this.AddCell(row, val3);
        return row;
    };

    TableScreen.prototype.AddCell = function (row, text) {
        var cell = document.createElement("td");
        cell.appendChild(document.createTextNode(text));
        row.appendChild(cell);
    };
    return TableScreen;
})();

var mainscreen;
var tablesceen;

function clearDataElement() {
    var dataElement = document.getElementById('data');
    while (dataElement.firstChild) {
        dataElement.removeChild(dataElement.firstChild);
    }
}
function selectHandler(e) {
    var data = mainscreen.data[mainscreen.chart.getSelectedIndex() + 1];
    clearDataElement();
    var dataElement = document.getElementById('data');
    var paragraph = document.createElement("p");
    paragraph.appendChild(document.createTextNode(data[0] + ": " + data[1] + " € "));
    var showBtn = document.createElement("span");
    showBtn.setAttribute("id", "showbtn");
    showBtn.setAttribute("onClick", "setTableScreen()");
    showBtn.appendChild(document.createTextNode("Show →"));
    paragraph.appendChild(showBtn);
    dataElement.appendChild(paragraph);
}

function getElement(list) {
    var index = Math.floor(Math.random() * list.length);
    return list[index];
}

var firstyear = 2011;
var lastyear = 2013;
var year = 2013;
var month = 0;
var expenses;
var types = ['Food', 'Transportation', 'Clothes', 'Housing', 'Housing costs'];
var food = ["Edeka", "Aldi", "Lidl", "Netto", "InterSpar"];
var transportation = ["OMW", "Deutsche Bahn", "MOL", "BKV"];
var clothes = ["C & A", "H & M", "Charles Vögele", "Kaufhof"];
var housing = ["Frau Müller"];
var housingCosts = ["EON", "1 und 1"];
var shopLists = [food, transportation, clothes, housing, housingCosts];

function generateExpenses() {
    var expenses = [[], [], [], [], []];

    var salary = 1000;
    while (salary > 0) {
        var type = Math.floor(Math.random() * shopLists.length);
        var amount = Math.floor(Math.random() * 100) / 2;
        var day = Math.floor(Math.random() * 28) + 1;
        expenses[type].push([day, getElement(shopLists[type]), amount]);
        salary -= amount;
    }
    return expenses;
}

function aggregateExpenses(expenses) {
    var data = [
        ['Type', 'Euro in month']
    ];

    for (var i = 0; i < types.length; ++i) {
        var explist = expenses[i];
        explist.sort(function (a, b) {
            return a[0] - b[0];
        });
        var amount = 0;
        for (var j = 0; j < explist.length; ++j) {
            amount += explist[j][2];
        }
        data.push([types[i], amount]);
    }
    return data;
}

function main() {
    expenses = [];
    var monthcount = (lastyear - firstyear + 1) * 12;
    for (var i = 0; i < monthcount; ++i)
        expenses[i] = generateExpenses();
    var data = aggregateExpenses(expenses[month]);
    mainscreen = new MainScreen(data, document.getElementById('mainscreen'), selectHandler);
}

function setMainScreen() {
    var screen = document.getElementById('screen');
    screen.removeChild(tablesceen.element);
    screen.appendChild(mainscreen.element);
}

function setTableScreen() {
    var index = mainscreen.chart.getSelectedIndex();
    tablesceen = new TableScreen(expenses[getMonthIndex()][index], types[index]);
    var screen = document.getElementById('screen');
    screen.removeChild(mainscreen.element);
    screen.appendChild(tablesceen.element);
}

function getMonthIndex() {
    return (year - firstyear) * 12 + month;
}

function refreshChart() {
    var data = aggregateExpenses(expenses[getMonthIndex()]);
    mainscreen = new MainScreen(data, document.getElementById('mainscreen'), selectHandler);
}

function setMonth(m) {
    var monthBtn = document.getElementById('m' + month);
    monthBtn.removeAttribute("class");
    month = m;
    monthBtn = document.getElementById('m' + month);
    monthBtn.setAttribute("class", "selected");
    refreshChart();
}

function setYear() {
    var yearlabel = document.getElementById("year");
    yearlabel.removeChild(yearlabel.firstChild);
    yearlabel.appendChild(document.createTextNode("" + year));
    refreshChart();
}

function deactivate(button) {
    button.removeAttribute("onClick");
    button.setAttribute("class", "inactive");
}

function activate(button, callback) {
    button.setAttribute("onClick", callback);
    button.setAttribute("class", "active");
}

function setRightArrow(button) {
    var cl = button.getAttribute("class");
    button.setAttribute("class", "fa fa-chevron-right " + cl);
}

function setLeftArrow(button) {
    var cl = button.getAttribute("class");
    button.setAttribute("class", "fa fa-chevron-left " + cl);
}

function decreaseYear() {
    if (year == lastyear) {
        var yearinc = document.getElementById("yearinc");
        activate(yearinc, "increaseYear()");
        setRightArrow(yearinc);
    }
    --year;
    if (year == firstyear) {
        var yeardec = document.getElementById("yeardec");
        deactivate(yeardec);
        setLeftArrow(yeardec);
    }
    setYear();
}

function increaseYear() {
    if (year == firstyear) {
        var yeardec = document.getElementById("yeardec");
        activate(yeardec, "decreaseYear()");
        setLeftArrow(yeardec);
    }
    ++year;
    if (year == lastyear) {
        var yearinc = document.getElementById("yearinc");
        deactivate(yearinc);
        setRightArrow(yearinc);
    }
    setYear();
}
//# sourceMappingURL=platandemo.js.map
