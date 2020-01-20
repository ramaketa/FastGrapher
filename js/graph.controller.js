const graphApp = angular.module('graphApp', []);

graphApp.controller('graphController', function($scope, $http, graphParamsService){
   const vm = $scope;
   let varsString = '';
   let vars = [];

   vm.newItem = {
   };
   vm.graph = {
   };

    vm.variables = [];

    vm.addVariable = (data) => {
        if (data.term && data.value) {
            vm.variables.push(data);
        } else {
            alert('Заполните все поля!');
        }
        vm.newItem = {};
    };

    vm.removeItem = (index) => {
        vm.variables.splice(index, 1);
    };

    vm.sendParams = () => {
        vars = '';
        varsString = '';
        if(vm.graphForm.$valid) {
            for(let i = 0; i < vm.variables.length; i++) {
                varsString += vm.variables[i].term + ',';
                if (i !== vm.variables.length - 1) {
                    vars += Number(vm.variables[i].value) + ' ';
                } else {
                    vars += Number(vm.variables[i].value);
                }
            }
            varsString += 'x';

            graphParamsService.sendRequest(vm.graph.equation, varsString, vars, vm.graph.min, vm.graph.max, vm.graph.step)
                .then(
                    (res) => {
                        let responseData = res.data;
                        let data_set = responseData.x.map(
                            (item)=>{
                                return Number(item.toFixed(4));
                            });
                        let label_set = responseData.y.map(
                            (item)=>{
                                return Number(item.toFixed(4));
                        });
                        const chartElem = document.getElementById("myChart");
                        new Chart(chartElem, {
                            type: 'line',
                            data: {
                                labels: data_set,
                                datasets: [{
                                    data: label_set,
                                    label: "y = f(x)",
                                    borderColor: "#3e95cd",
                                    fill: false
                                }
                                ]
                            },
                            options: {
                                title: {
                                    display: true,
                                    text: 'function y = f(x)'
                                },
                                scales: {
                                    yAxes:[{
                                        gridLines: {
                                            display:true,
                                            color:"rgba(255,99,132,0.2)"
                                        },
                                    }],
                                    xAxes:[{
                                        gridLines: {
                                            display:true
                                        },
                                    }]
                                }
                            }
                        });
                        chartElem.scrollIntoView({block: "center", behavior: "smooth"});
                    },
                    (err) => {
                        alert("Извините, что-то пошло не так. Проверьте корректность данных или обратитесь к администратору")
                        console.log(err)
                    }
                )
        } else {
            alert('Заполните все поля!');
        }
    }
});

graphApp.service('graphParamsService', function($http){
    const service = {};

    service.sendRequest = (equation, varsString, vars, min, max, step) => {
        const data = {
            "equation": equation,
            "varsString": varsString,
            "vars": vars,
            "min": Number(min),
            "max": Number(max),
            "step": Number(step),
        };
        return $http({
            method: 'POST',
            url: 'http://localhost:3000',
            headers: {
                'Content-Type': 'application/json',
            },
            data: data,
        });
    };

    return service;
});