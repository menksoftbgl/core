# -*- Mode: python; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

from uitest_helper import UITest

from helper import mkPropertyValues

from uihelper.calc import enter_text_to_cell

from UITestCase import UITestCase

import time

try:
    import pyuno
    import uno
    import unohelper
except ImportError:
    print("pyuno not found: try to set PYTHONPATH and URE_BOOTSTRAP variables")
    print("PYTHONPATH=/installation/opt/program")
    print("URE_BOOTSTRAP=file:///installation/opt/program/fundamentalrc")
    raise

class GridWindowTest(UITestCase):

    def test_input(self):

        self.ui_test.create_doc_in_start_center("calc")
        xTopWindow = self.xUITest.getTopFocusWindow()

        xGridWindow = xTopWindow.getChild("grid_window")

        enter_text_to_cell(xGridWindow, "C3", "=A1")
        enter_text_to_cell(xGridWindow, "A1", "2")

        self.ui_test.close_doc()

    def test_special_keys(self):

        self.ui_test.create_doc_in_start_center("calc")
        xTopWindow = self.xUITest.getTopFocusWindow()

        xGridWindow = xTopWindow.getChild("grid_window")

        selectProps = mkPropertyValues({"CELL": "C3"})
        xGridWindow.executeAction("SELECT", selectProps)

        typeProps = mkPropertyValues({"KEYCODE": "CTRL+DOWN"})
        xGridWindow.executeAction("TYPE", typeProps)

        self.ui_test.close_doc()

# vim:set shiftwidth=4 softtabstop=4 expandtab: */
