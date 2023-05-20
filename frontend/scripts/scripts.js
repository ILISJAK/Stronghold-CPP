$(document).ready(function () {
    const baseUrl = 'http://localhost:PORT'; // Replace PORT with the actual port your API is running on.

    $('#create-town').click(function () {
        $.post(baseUrl + '/api/town/create', { name: 'Town A' }, function (data) {
            $('#town-info').html(`<p>Town created: ${data.name}</p>`);
        });
    });

    $('#raid-town').click(function () {
        $.post(baseUrl + '/api/town/raid', { townId: 1 }, function (data) {
            $('#town-info').html(`<p>Town raided: ${data.message}</p>`);
        });
    });

    $('#train-troops').click(function () {
        $.post(baseUrl + '/api/troops/train', { townId: 1, troopType: 'archer', count: 5 }, function (data) {
            $('#town-info').html(`<p>Troops trained: ${data.count} ${data.troopType}s</p>`);
        });
    });
});
