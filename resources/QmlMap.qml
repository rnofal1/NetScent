import QtQuick 2.15
import QtPositioning 5.11
import QtLocation 6.6

Item {
    id:window
    property double latitude: 48.8566; //Random default value
    property double longitude: 2.3522; //Random default value
    property Component location_marker: loc_marker

    MapView {
        id: mapview
        anchors.fill: parent
        map.plugin: Plugin {name: "osm" } //Open Street Map
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

    function set_location_marker(mark_lat, mark_long, loc_text) {
        var item = location_marker.createObject(window, {
                                                   coordinate:QtPositioning.coordinate(mark_lat, mark_long),
                                                   item_text: loc_text
                                               })
        mapview.map.addMapItem(item)
    }

    function remove_all_location_markers() {
        mapview.map.clearMapItems()
    }

    Component {
        id:loc_marker
        MapQuickItem {
            id:markerImg
            anchorPoint.x: image.width/2
            anchorPoint.y: image.height/2
            coordinate: position
            property string item_text
            sourceItem: Image {
                id: image
                width: 20
                height: 20
                source: "https://www.pngarts.com/files/3/Map-Marker-Pin-PNG-Picture.png"
                anchors.centerIn: parent
                Rectangle {
                    id: imagePane
                    visible: false
                    color: "#33000000"
                    height:  imageText.contentHeight
                    width:  imageText.contentWidth
                    anchors.left: image.right
                    anchors.top: image.top
                    Text {
                        id: imageText
                        anchors.fill: parent
                        text: item_text
                        color: "black"
                        font.pointSize: 5
                    }
                }
                HoverHandler {
                    onHoveredChanged: {
                        if(hovered) {
                            image.scale = 2
                            imagePane.visible = true
                            markerImg.anchorPoint.x = image.width
                            markerImg.anchorPoint.y = image.height
                        } else {
                            image.scale = 1
                            imagePane.visible = false
                            markerImg.anchorPoint.x = image.width/2
                            markerImg.anchorPoint.y = image.height/2
                        }
                    }
                }
            }
        }
    }
}
