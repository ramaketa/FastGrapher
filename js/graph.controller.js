const graphApp = angular.module('graphApp', []);

graphApp.controller('graphController', function($scope, graphParamsService){
   const vm = $scope;

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
        if(vm.graphForm.$valid) {
            console.log(vm.graph, vm.variables);
            graphParamsService.sendRequest(vm.graph)
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

graphApp.service('graphParamsService', function(){
    const service = undefined;

    service.sendRequest = (data) => {
        $http({
            method: 'GET',
            url: 'http://localhost:3000',
            params: data,
        });
    };

    return service;
});