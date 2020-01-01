const graphApp = angular.module('graphApp', []);

graphApp.controller('graphController', function($scope){
    'ngInject';

    const vm = $scope;

    vm.name = 'Mandolorian';
});