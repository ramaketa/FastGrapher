const graphApp = angular.module('graphApp', []);

graphApp.controller('graphController', function($scope, graphParamsService){
   const vm = $scope;

   vm.newItem = {
   };

    vm.items = graphParamsService.allVars;

    vm.addVariable = (data) => {
        if (data.term && data.value) {
            vm.items.push(data);
        } else {
            alert('Заполните все поля!');
        }
        vm.newItem = {};
    };

    vm.removeItem = (index) => {
        vm.items.splice(index, 1);
    };

    vm.sendParams = () => {

    }
});

graphApp.service('graphParamsService', function(){
    return {
        allVars: [
        ]
    }
});