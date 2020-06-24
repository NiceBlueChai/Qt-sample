#pragma once

#include "ui_ble.h"
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QtWidgets/QWidget>
namespace UI
{
class BLEClass;
}
class QStatusBar;
class QLowEnergyController;
class BLE : public QWidget
{
	Q_OBJECT

public:
	BLE(QWidget *parent = Q_NULLPTR);
private Q_SLOTS:
	void on_btnScanf_clicked();
	void on_btnConnect_clicked();
	void on_btnDisConnect_clicked();
	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

	void on_deviceDiscovered(const QBluetoothDeviceInfo &info);
	void on_deviceDiscoverFinished();
	void on_deviceDiscoverError(QBluetoothDeviceDiscoveryAgent::Error error);

	void on_controlStateChanged(QLowEnergyController::ControllerState state);
	void on_serviceDiscovered(const QBluetoothUuid &newService);
	void on_serviceDiscoveryFinished();

	void on_serviceStateChanged(QLowEnergyService::ServiceState newState);
	void on_detialsDiscovered(QLowEnergyService *service);

private:
	void initUI();
	void initSignalSlot();

private:
	Ui::BLEClass ui;
	QStatusBar *m_statusBar;
	QLowEnergyController *m_control;
	QList<QLowEnergyService *> m_serviceList;
	QBluetoothDeviceDiscoveryAgent m_agent;
	QList<QBluetoothDeviceInfo> m_deviceList;
};
