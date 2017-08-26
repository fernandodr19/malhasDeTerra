
<div id="tabheader">
        <ul id="tablist">
            <li id="addprop" style="background: #aaa"><a href="javascript:activateTab('tablist','addprop','tabcontent','addprop_tab')" title="">Add Property</a></li>
            <li id="savedprop"><a href="javascript:activateTab('tablist','savedprop','tabcontent','savedprop_tab')" title="">Saved Property</a></li>
            <li id="inbox"><a href="javascript:activateTab('tablist','inbox','tabcontent','inbox_tab')" title="">Inbox</a></li>
            <li id="profile"><a href="javascript:activateTab('tablist','profile','tabcontent','profile_tab')" title="">Update Profile</a></li>
        </ul>
    </div> <!-- End of tabheader -->
    <div id="tabcontent" class="maintab">
        <div id="addprop_tab" style="display: block">
            <p><a href="#" title="Click to add property">Add your property to NPLH</a></p>
        </div> <!-- End of addprop_tab -->
        <div id="savedprop_tab" style="display: none">
            <p>Browse your saved properties</p>
        </div> <!-- End of savedprop_tab -->
        <div id="inbox_tab" style="display: none">
            <p>inbox</p>
        </div> <!-- End of inbox_tab -->
        <div id="profile_tab" style="display: none">
            <p>
                <ul>
                    <a href="javascript:activateTab('tablist','profile','profile_subtab','tab1')" title="">Tab1</a>
                    <a href="javascript:activateTab('tablist','profile','profile_subtab','tab2')" title="">Tab2</a>
                    <a href="javascript:activateTab('tablist','profile','profile_subtab','tab3')" title="">Tab3</a>
                </ul>
            </p>
            <div id="profile_subtab" class="subtab">
                <div id="tab1" style="display: block">
                    <p>tab1</p>
                </div> <!-- End of tab1 -->
                <div id="tab2" style="display: none">
                    <p>tab2</p>
                </div> <!-- End of tab2 -->
                <div id="tab3" style="display: none">
                    <p>tab3</p>
                </div> <!-- End of tab3 -->
            </div>
        </div> <!-- End of profile_tab -->
    </div> <!-- End of tabcontent -->

<script type="text/javascript">
    function activateTab(mainid, li_id, divid, tabid) {
        var mainDiv = document.getElementById(mainid);
        var li = document.getElementById(li_id);
        var tabDiv = document.getElementById(divid);
        var mainTab = document.getElementById(tabid);

        for (var i = 0; i < mainDiv.childNodes.length; i++) {
            var node = mainDiv.childNodes[i];
            if (node.nodeType == 1) {
                node.style.background = (node == li) ? '#aaa' : 'white';
            }
        }
        for (var i = 0; i < tabDiv.childNodes.length; i++) {
            var node = tabDiv.childNodes[i];
            if (node.nodeType == 1) {
                node.style.display = (node == mainTab) ? 'block' : 'none';
            }
        }
    } 
</script>