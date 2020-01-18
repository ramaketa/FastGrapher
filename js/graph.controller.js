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
        vars = [];
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

            console.log(varsString, vars);
            graphParamsService.sendRequest(vm.graph.equation, varsString, vars, vm.graph.min, vm.graph.max, vm.graph.step)
                .then(
                    (res) => {
                        console.log(res);
                    },
                    (err) => {
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
            "vars": vars.toString(),
            "min": min,
            "max": max,
            "step": step,
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