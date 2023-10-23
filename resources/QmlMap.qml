import QtQuick 2.15
import QtPositioning 5.11
import QtLocation 6.6

Item {
    id:window
    property double latitude: 48.8566;
    property double longitude: 2.3522;
    property Component location_marker: loc_marker

    MapView {
        id: mapview
        anchors.fill: parent
        map.plugin: Plugin {name: "osm" }
        map.center: QtPositioning.coordinate(latitude, longitude)
        map.zoomLevel: 10
    }

    function set_map_center(new_lat, new_long) {
        mapview.map.pan(latitude - new_lat, longitude - new_long)
        latitude = new_lat
        longitude= new_long
    }

    function set_map_zoom(zoom_level) {
        mapview.map.zoomLevel = zoom_level
    }

    function set_location_marker(mark_lat, mark_long) {
        var item = location_marker.createObject(window, {
                                                   coordinate:QtPositioning.coordinate(mark_lat, mark_long)
                                               })
        mapview.map.addMapItem(item)

    }

    Component {
        id:loc_marker
        MapQuickItem {
            id:markerImg
            anchorPoint.x: image.width/4
            anchorPoint.y: image.height/4
            coordinate: position
            sourceItem: Image {
                id: image
                width: 20
                height: 20
                source: "https://www.pngarts.com/files/3/Map-Marker-Pin-PNG-Picture.png"
            }
        }
    }
}
