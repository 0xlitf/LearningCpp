import QtQuick
import QtQuick.Window
import QtQuick.Particles
import QtWebEngine

Window {
    id: window
    width: 1366
    height: 900
    visible: true
    title: qsTr("Hello World")

    WebEngineView {
        id: webView
        width: parent.width
        height: parent.height
        backgroundColor: "transparent"
        anchors.centerIn: parent
        settings.javascriptEnabled: true
        settings.pluginsEnabled: true
        url: "https://verytoolz.com/xml-parser.html"
        onLoadProgressChanged: {
            console.log("loadProgressd: ", progress)
        }
        onCertificateError: {
            console.log("onCertificateError: ", error)
        }
        onLoadingChanged: {
            console.log("loadingInfo: ", loadingInfo)
        }
        onRecommendedStateChanged: {
            console.log("recommendedState: ", webView.recommendedState)
        }

        Component.onCompleted: {
            console.log("WebEngineView.LoadStartedStatus: ", WebEngineView.LoadStartedStatus)
        }
    }
}
