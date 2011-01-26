/***************************************************************************
 * gens-qt4: Gens Qt4 UI.                                                  *
 * GeneralConfigWindow.hpp: General Configuration Window.                  *
 *                                                                         *
 * Copyright (c) 1999-2002 by Stéphane Dallongeville.                      *
 * Copyright (c) 2003-2004 by Stéphane Akhoun.                             *
 * Copyright (c) 2008-2011 by David Korth.                                 *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation; either version 2 of the License, or (at your  *
 * option) any later version.                                              *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.           *
 ***************************************************************************/

#ifndef __GENS_QT4_GENERALCONFIGWINDOW_HPP__
#define __GENS_QT4_GENERALCONFIGWINDOW_HPP__

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include "ui_GeneralConfigWindow.h"

#include "GensLineEdit.hpp"
#include <stdio.h>

// libgens: Sega CD Boot ROM database.
#include "libgens/Data/mcd_rom_db.h"

namespace GensQt4
{

class GeneralConfigWindow : public QDialog, public Ui::GeneralConfigWindow
{
	Q_OBJECT
	
	public:
		static void ShowSingle(QWidget *parent = NULL);
	
	protected:
		GeneralConfigWindow(QWidget *parent = NULL);
		virtual ~GeneralConfigWindow();
		
		void setApplyButtonEnabled(bool enabled)
		{
			QPushButton *btnApply = buttonBox->button(QDialogButtonBox::Apply);
			if (btnApply)
				btnApply->setEnabled(enabled);
		}
		
		// Sega CD: Boot ROM file textboxes.
		GensLineEdit *txtMcdRomUSA;
		GensLineEdit *txtMcdRomEUR;
		GensLineEdit *txtMcdRomJPN;
	
	protected slots:
		void accept(void) { apply(); QDialog::accept(); }
		void apply(void);
		
		void reload(void);
	
	private:
		static GeneralConfigWindow *m_GeneralConfigWindow;
		
		// Sega CD: Boot ROM filesize.
		static const int MCD_ROM_FILESIZE = 131072;
		
		// Sega CD: Select Boot ROM file.
		void mcdSelectRomFile(const QString& rom_id, GensLineEdit *txtRomFile);
		
		// Sega CD: Update Boot ROM file status.
		QString mcdUpdateRomFileStatus(GensLineEdit *txtRomFile, int region_code);
		
		// Sega CD: Display Boot ROM file status.
		void mcdDisplayRomFileStatus(const QString& rom_id, const QString& rom_desc);
		
		// Sega CD: Boot ROM file information.
		QString sMcdRomStatus_USA;
		QString sMcdRomStatus_EUR;
		QString sMcdRomStatus_JPN;
	
	private slots:
		void on_btnMcdRomUSA_clicked(void);
		void on_btnMcdRomEUR_clicked(void);
		void on_btnMcdRomJPN_clicked(void);
		
		void on_txtMcdRomUSA_focusIn(void);
		void on_txtMcdRomEUR_focusIn(void);
		void on_txtMcdRomJPN_focusIn(void);
		
		void on_txtMcdRomUSA_textChanged(void);
		void on_txtMcdRomEUR_textChanged(void);
		void on_txtMcdRomJPN_textChanged(void);
};

}

#endif /* __GENS_QT4_GENERALCONFIGWINDOW_HPP__ */
