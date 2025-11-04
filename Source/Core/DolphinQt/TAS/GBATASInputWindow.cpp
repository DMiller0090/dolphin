// Copyright 2022 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "DolphinQt/TAS/GBATASInputWindow.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QSpinBox>
#include <QVBoxLayout>

#include "Common/CommonTypes.h"

#include "Core/HW/GBAPad.h"
#include "Core/HW/GBAPadEmu.h"

#include "DolphinQt/TAS/TASCheckBox.h"

#include "InputCommon/ControllerEmu/ControllerEmu.h"
#include "InputCommon/InputConfig.h"

GBATASInputWindow::GBATASInputWindow(QWidget* parent, int controller_id)
    : TASInputWindow(parent), m_controller_id(controller_id)
{
  setWindowTitle(tr("GBA TAS Input %1").arg(controller_id + 1));

  m_b_button =
      CreateButton(QStringLiteral("&B"), GBAPad::BUTTONS_GROUP, GBAPad::B_BUTTON, &m_overrider);
  m_a_button =
      CreateButton(QStringLiteral("&A"), GBAPad::BUTTONS_GROUP, GBAPad::A_BUTTON, &m_overrider);
  m_l_button =
      CreateButton(QStringLiteral("&L"), GBAPad::BUTTONS_GROUP, GBAPad::L_BUTTON, &m_overrider);
  m_r_button =
      CreateButton(QStringLiteral("&R"), GBAPad::BUTTONS_GROUP, GBAPad::R_BUTTON, &m_overrider);
  m_select_button = CreateButton(QStringLiteral("SELE&CT"), GBAPad::BUTTONS_GROUP,
                                 GBAPad::SELECT_BUTTON, &m_overrider);
  m_start_button = CreateButton(QStringLiteral("&START"), GBAPad::BUTTONS_GROUP,
                                GBAPad::START_BUTTON, &m_overrider);

  m_left_button =
      CreateButton(QStringLiteral("L&eft"), GBAPad::DPAD_GROUP, DIRECTION_LEFT, &m_overrider);
  m_up_button = CreateButton(QStringLiteral("&Up"), GBAPad::DPAD_GROUP, DIRECTION_UP, &m_overrider);
  m_down_button =
      CreateButton(QStringLiteral("&Down"), GBAPad::DPAD_GROUP, DIRECTION_DOWN, &m_overrider);
  m_right_button =
      CreateButton(QStringLiteral("R&ight"), GBAPad::DPAD_GROUP, DIRECTION_RIGHT, &m_overrider);

  auto* buttons_layout = new QGridLayout;

  buttons_layout->addWidget(m_left_button, 0, 0);
  buttons_layout->addWidget(m_up_button, 0, 1);
  buttons_layout->addWidget(m_down_button, 0, 2);
  buttons_layout->addWidget(m_right_button, 0, 3);

  buttons_layout->addWidget(m_l_button, 1, 0);
  buttons_layout->addWidget(m_r_button, 1, 1);
  buttons_layout->addWidget(m_b_button, 1, 2);
  buttons_layout->addWidget(m_a_button, 1, 3);

  buttons_layout->addWidget(m_select_button, 2, 0, 1, 2);
  buttons_layout->addWidget(m_start_button, 2, 2, 1, 2);

  buttons_layout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding), 0, 4);

  QGroupBox* buttons_box = new QGroupBox(tr("Buttons"));
  buttons_box->setLayout(buttons_layout);

  m_disconnect_checkbox = new QCheckBox(tr("Disconnect link cable"));
  m_disconnect_checkbox->setToolTip(
      tr("While checked, the GBA link cable is force-disconnected for this port "
         "(records via the Y button bit)."));
  connect(m_disconnect_checkbox, &QCheckBox::toggled, this,
          &GBATASInputWindow::OnDisconnectToggled);

  // Ensure m_settings_box has a layout and add the checkbox there
  if (!m_settings_box->layout())
    m_settings_box->setLayout(new QVBoxLayout);
  m_settings_box->layout()->addWidget(m_disconnect_checkbox);

  auto* layout = new QVBoxLayout;
  layout->addWidget(buttons_box);
  layout->addWidget(m_settings_box);

  setLayout(layout);
}
void GBATASInputWindow::OnDisconnectToggled(bool checked)
{
  Pad::SetGBAForceDisconnect(m_controller_id, checked);
}
void GBATASInputWindow::showEvent(QShowEvent* event)
{
  Pad::GetGBAConfig()
      ->GetController(m_controller_id)
      ->SetInputOverrideFunction(m_overrider.GetInputOverrideFunction());

  m_disconnect_checkbox->setChecked(Pad::GetGBAForceDisconnect(m_controller_id));

  TASInputWindow::showEvent(event);
}
void GBATASInputWindow::hideEvent(QHideEvent* event)
{
  Pad::GetGBAConfig()->GetController(m_controller_id)->ClearInputOverrideFunction();
}

