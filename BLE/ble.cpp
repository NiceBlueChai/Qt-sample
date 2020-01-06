#pragma execution_character_set("utf-8")
#include "ble.h"
#include <QLowEnergyController>
#include <QMessageBox>
#include <QStatusBar>
#include <QDebug>
BLE::BLE(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initUI();
	initSignalSlot();

	m_control = nullptr;
	m_agent.setLowEnergyDiscoveryTimeout(10 * 1000);

}
void BLE::on_btnScanf_clicked()
{
	ui.listWidget->clear();
	m_deviceList.clear();

	m_statusBar->showMessage("正在扫描设备...");
	m_agent.start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void BLE::on_btnConnect_clicked()
{
	ui.listWidget->addItem("con");
}

void BLE::on_btnDisConnect_clicked()
{
	m_control->disconnectFromDevice();
	ui.btnDisConnect->setEnabled(false);
}
void BLE::on_listWidget_itemDoubleClicked(QListWidgetItem* item)
{
	if (ui.btnDisConnect->isEnabled())
	{
		on_btnDisConnect_clicked();
	}
	ui.treeWidget->clear();
	int index = ui.listWidget->currentIndex().row();
	m_control = QLowEnergyController::createCentral(m_deviceList.at(index));
	assert(m_control);
	connect(m_control, &QLowEnergyController::stateChanged, this, &BLE::on_controlStateChanged);
	m_control->connectToDevice();
	
}
void BLE::on_deviceDiscovered(const QBluetoothDeviceInfo& info)
{
	m_statusBar->showMessage("发现设备:" + info.name());
}
void BLE::on_deviceDiscoverFinished()
{
	m_statusBar->showMessage("设备扫描完毕");
	for (auto device : m_agent.discoveredDevices())
	{
		m_deviceList.append(device);
		ui.listWidget->addItem(device.name());
	}
}
void BLE::on_deviceDiscoverError(QBluetoothDeviceDiscoveryAgent::Error error)
{
	qDebug() << error;
	QMessageBox::information(this, "error", "error");
}
void BLE::on_controlStateChanged(QLowEnergyController::ControllerState state)
{
	switch (state)
	{
	case QLowEnergyController::UnconnectedState:
		m_statusBar->showMessage("设备未连接");
		break;
	case QLowEnergyController::ConnectingState: {
		m_statusBar->showMessage("正在连接设备...");
		ui.btnDisConnect->setEnabled(true);
		break; }
	case QLowEnergyController::ConnectedState: {
		ui.btnDisConnect->setEnabled(true);
		m_statusBar->showMessage("设备已连接:" + m_control->remoteName());
		connect(m_control, &QLowEnergyController::serviceDiscovered, this, &BLE::on_serviceDiscovered);
		connect(m_control, &QLowEnergyController::discoveryFinished, this, &BLE::on_serviceDiscoveryFinished);
		m_control->discoverServices();
		break; }
	case QLowEnergyController::DiscoveringState:
		m_statusBar->showMessage("正在发现服务...");
		break;
	case QLowEnergyController::DiscoveredState:
		m_statusBar->showMessage("服务扫描完毕");
		break;
	case QLowEnergyController::ClosingState:
		m_statusBar->showMessage("设备已关闭");
		break;
	case QLowEnergyController::AdvertisingState:
		m_statusBar->showMessage("正在广播");
		break;
	default:
		break;
	}
}
void BLE::on_serviceDiscovered(const QBluetoothUuid& newService)
{
	m_statusBar->showMessage("发现服务:" + newService.toString());
}
void BLE::on_serviceDiscoveryFinished()
{
	m_serviceList.clear();
	for (auto uid:m_control->services())
	{
		auto service = m_control->createServiceObject(uid, this);
		m_serviceList.append(service);
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0, service->serviceName());
		item->setText(1, service->serviceUuid().toString());
		item->setData(1, Qt::UserRole, service->serviceUuid());
		ui.treeWidget->addTopLevelItem(item);
	}
	for (QLowEnergyService* lowEnergyService : m_serviceList) {
		connect(lowEnergyService, 
			QOverload<QLowEnergyService::ServiceError>::of(&QLowEnergyService::error),
			[=](QLowEnergyService::ServiceError newError) { 
			qDebug() << __FUNCTION__ << newError << lowEnergyService->serviceName();
		});
		connect(lowEnergyService, &QLowEnergyService::stateChanged, this,
			&BLE::on_serviceStateChanged);
		if (lowEnergyService->state() == QLowEnergyService::DiscoveryRequired)
		{
			lowEnergyService->discoverDetails();	
		}
	}

}
void BLE::on_serviceStateChanged(QLowEnergyService::ServiceState newState)
{
	QLowEnergyService* service = static_cast<QLowEnergyService*>(sender());
	if (!service)
	{
		qInfo() << "ERROR :" << __FUNCTION__;
		return;
	}
	switch (newState)
	{
	case QLowEnergyService::InvalidService:
		m_statusBar->showMessage(service->serviceName() + "Invalid...");
		break;
	case QLowEnergyService::DiscoveryRequired:
		m_statusBar->showMessage(service->serviceName() + "Required...");
		break;
	case QLowEnergyService::DiscoveringServices:
		m_statusBar->showMessage(service->serviceName() + "Discovering...");
		break;
	case QLowEnergyService::ServiceDiscovered: {
		m_statusBar->showMessage(service->serviceName() + "Discovered...");
		on_detialsDiscovered(service);
		break; }
	case QLowEnergyService::LocalService:
		m_statusBar->showMessage(service->serviceName() + "Local...");
		break;
	default:
		break;
	}
}
void BLE::on_detialsDiscovered(QLowEnergyService * service)
{
	int index = m_serviceList.indexOf(service);
	auto topItem = ui.treeWidget->topLevelItem(index);
	for (QLowEnergyCharacteristic characteristic: service->characteristics())
	{
		auto item = new QTreeWidgetItem;
		item->setText(0, characteristic.name());
		item->setText(1, characteristic.value().toHex('-'));
		item->setData(0, Qt::UserRole, characteristic.uuid());
		item->setData(1, Qt::UserRole, characteristic.handle());
		item->setData(1, Qt::UserRole + 1, characteristic.value());
		topItem->addChild(item);
		
	}

}
void BLE::initUI()
{
	ui.btnConnect->setEnabled(false);
	ui.btnDisConnect->setEnabled(false);
	m_statusBar = new QStatusBar(ui.widget);
	m_statusBar->setMinimumWidth(400);
	QHBoxLayout* layout = new QHBoxLayout(ui.widget);
	ui.widget->setLayout(layout);
	m_statusBar->showMessage("准备就绪");

	QStringList headers;
	headers << "Name" << "Uuid";
	ui.treeWidget->setHeaderLabels(headers);
	ui.treeWidget->setColumnWidth(0, 200);
}
void BLE::initSignalSlot()
{
	connect(&m_agent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)), this,
		SLOT(on_deviceDiscovered(const QBluetoothDeviceInfo&)));
	connect(&m_agent, SIGNAL(finished()), this, SLOT(on_deviceDiscoverFinished()));
	connect(&m_agent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
		this, SLOT(on_deviceDiscoverError(QBluetoothDeviceDiscoveryAgent::Error)));
}