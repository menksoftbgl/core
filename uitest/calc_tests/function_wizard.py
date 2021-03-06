# -*- Mode: python; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

from uitest_helper import UITest

from helper import mkPropertyValues
import time

from UITestCase import UITestCase

try:
    import pyuno
    import uno
    import unohelper
except ImportError:
    print("pyuno not found: try to set PYTHONPATH and URE_BOOTSTRAP variables")
    print("PYTHONPATH=/installation/opt/program")
    print("URE_BOOTSTRAP=file:///installation/opt/program/fundamentalrc")
    raise

class FunctionWizardTest(UITestCase):
    # tdf#98427
    def test_open_function_wizard(self):
        self.ui_test.create_doc_in_start_center("calc")

        self.ui_test.execute_modeless_dialog_through_command(".uno:FunctionDialog")

        xFunctionDlg = self.xUITest.getTopFocusWindow()

        xArrayChkBox = xFunctionDlg.getChild("array")
        xArrayChkBox.executeAction("CLICK", tuple())

        xCancelBtn = xFunctionDlg.getChild("cancel")
        xCancelBtn.executeAction("CLICK", tuple())

        self.ui_test.close_doc()

# vim:set shiftwidth=4 softtabstop=4 expandtab: */
