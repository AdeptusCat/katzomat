import Paho from 'paho-mqtt'; //https://www.eclipse.org/paho/files/jsdoc/index.html

import {useState, useEffect} from 'react';
//import { StatusBar } from 'expo-status-bar';
import {
  StyleSheet,
  Text,
  Button,
  View,
  Image,
  ScrollView,
  Dimensions,
  Alert,
  Modal,
  Pressable,
  TouchableHighlight,
  Platform,
  TouchableOpacitiy,
  TextInput,
} from 'react-native';
import DateTimePicker from '@react-native-community/datetimepicker';
import {DateTimePickerAndroid} from '@react-native-community/datetimepicker';
import React from 'react';
import {SafeAreaProvider, SafeAreaView} from 'react-native-safe-area-context';
import {NavigationContainer} from '@react-navigation/native';
import {NativeStackExample} from './screens/NativeStackExample.tsx';
import {BottomTabNavigator} from './screens/utils/BottomTabNavigator.tsx';
import Icon from 'react-native-vector-icons/AntDesign';
import Ionicons from 'react-native-vector-icons/Ionicons';

import {createBottomTabNavigator} from '@react-navigation/bottom-tabs';

import ImageSlider from './ImageSlider';

import EncryptedStorage from 'react-native-encrypted-storage';

const Tab = createBottomTabNavigator();
export const PageContext = React.createContext();
export const SettingsContext = React.createContext();

client = new Paho.Client(
  'adeptuscat.ddns.net',
  Number(9001),
  `mqtt-async-test-${parseInt(Math.random() * 100)}`,
);

const images = [
  'https://upload.wikimedia.org/wikipedia/commons/thumb/5/5d/Eclectus_roratus_in_Zoo_Augsburg.jpg/1280px-Eclectus_roratus_in_Zoo_Augsburg.jpg',
  'https://upload.wikimedia.org/wikipedia/commons/e/e0/Green-park.jpg',
  'https://upload.wikimedia.org/wikipedia/commons/2/26/Skyscraper_Los_Angeles_Downtown_2013.jpg',
];

const WIDTH = Dimensions.get('window').width;
const HEIGHT = Dimensions.get('window').height;
var imgKeys = [];
var dotKeys = [];

var once = false;

export default function App() {
  var statusString = 'Katzomat Status:';
  const [status, setStatus] = useState(`${statusString} unkown`);
  const [connected, setConnected] = useState(false);
  const [domain, setDomain] = useState('adeptuscat.ddns.net');
  const [name, setName] = useState('katz');
  const [password, setPassword] = useState('katz!');
  const [imgs, setImgs] = useState([
    {
      timestamp: 1661360552, //1661360551
      uri: 'https://upload.wikimedia.org/wikipedia/commons/thumb/5/5d/Eclectus_roratus_in_Zoo_Augsburg.jpg/1280px-Eclectus_roratus_in_Zoo_Augsburg.jpg',
      date: 'fsfasdf',
    },
  ]);
  const [timetable, setTimetable] = useState([]);
  const [rotationTime, setRotationTime] = useState(3000);
  const [value, setValue] = useState([1]);

  return (
    <PageContext.Provider
      value={{
        statusString,
        status,
        setStatus,
        connected,
        setConnected,
        name,
        setName,
        domain,
        setDomain,
        password,
        setPassword,
        imgs,
        setImgs,
        timetable,
        setTimetable,
        rotationTime,
        setRotationTime,
        value,
        setValue,
      }}>
      <Page />
    </PageContext.Provider>
  );
}

const Page = () => {
  const {
    statusString,
    status,
    setStatus,
    connected,
    setConnected,
    domain,
    setDomain,
    name,
    setName,
    password,
    setPassword,
    imgs,
    setImgs,
    timetable,
    setTimetable,
    rotationTime,
    setRotationTime,
  } = React.useContext(PageContext);
  const [value, setValue] = useState(0);
  //const [password, setPassword] = useState('katz!');
  //const [domain, setDomain] = useState('adeptuscat.ddns.net');
  const [dir, setDir] = useState(`katzomat/${name}`);
  //const [img, setImg] = useState('Base64ImgHere');
  //const [timetable, setTimetable] = useState([]);
  const [statusVerbose, setStatusVerbose] = useState({});
  function formatTime(timestamp) {
    var date = new Date(timestamp * 1000);
    var year = date.getFullYear();
    var month = date.getMonth();
    var day = String(date.getDate()).padStart(2, '0');
    var hours = date.getHours();
    var minutes = '0' + date.getMinutes();
    var seconds = '0' + date.getSeconds();
    var formattedTime =
      day +
      '.' +
      month +
      '.' +
      year +
      ' ' +
      hours +
      ':' +
      minutes.substr(-2) +
      ':' +
      seconds.substr(-2);
    return formattedTime;
  }

  function onMessage(message) {
    if (message.destinationName === `katzomat/${name}/status`) {
      setStatus(`${statusString} ${message.payloadString}`);
    }
    if (message.destinationName === `katzomat/${name}/status/verbose`) {
      //console.log(message.payloadString);
      var obj = JSON.parse(message.payloadString);
      setStatusVerbose(obj);
      console.log(message.payloadString);

      var dict = {
        time: '',
        history: [],
      };
      dict.time = formatTime(obj.time);
      for (let i = 0; i < obj.history.length; i++) {
        dict.history.push(formatTime(obj.history[i]));
      }
      console.log(dict);
    }
    const str = `katzomat/${name}/images`;
    const re = message.destinationName.match(str);
    if (re != null) {
      if (message.destinationName === `katzomat/${name}/images/take`) {
        console.log('take');
        return;
      }
      var obj = JSON.parse(message.payloadString);
      var base64Img = `data:image/png;base64,${obj.value}`;
      var timestamp = parseInt(obj.timestamp, 10);
      //setImg(base64Img);
      for (let i = 0; i < imgs.length; i++) {
        //console.log(timestamp);
        //console.log(imgs[i].timestamp);
        //console.log('next');
        if (imgs[i].timestamp === timestamp) {
          console.log('has img already');
          return;
        }
      }
      var date = new Date(timestamp * 1000);
      var year = date.getFullYear();
      var month = date.getMonth();
      var day = String(date.getDate()).padStart(2, '0');
      var hours = date.getHours();
      var minutes = '0' + date.getMinutes();
      var seconds = '0' + date.getSeconds();
      var formattedTime =
        day +
        '.' +
        month +
        '.' +
        year +
        ' ' +
        hours +
        ':' +
        minutes.substr(-2) +
        ':' +
        seconds.substr(-2);
      var arr = imgs;
      arr.push({
        timestamp: timestamp,
        uri: base64Img,
        date: formattedTime,
      });
      arr.sort((a, b) => a.timestamp - b.timestamp);

      //console.log(base64Img);
      //console.log(imgs);
      //console.log(timestamp);
      //setImgs([...imgs, n]);
      setImgs([...arr]);
      //imgs.map(a => console.log(a.date));

      //console.log(imgs);
      //var arr = imgss.append({"uri": "base64Img"});
      //console.log(arr);
      //var imgDict = imgs.append({uri: base64Img});
      //setImgs(imgDict);
      //const dict = {
      //  uri: base64Img,
      //};
      //imgs.append(dict);
    }
    if (message.destinationName === `katzomat/${name}/feed`) {
      //console.log(message.payloadString);
    }
    if (message.destinationName === `katzomat/${name}/timetable`) {
      console.log('timetable incomming');
      console.log(message.payloadString);
      var obj = JSON.parse(message.payloadString);
      var values = obj.values;
      parseTimetable(values);
    }
    if (message.destinationName === `katzomat/${name}/settings`) {
      setRotationTime(message.payloadString);
      saveDeviceSettings(message.payloadString);
    }
  }

  function parseTimetable(values) {
    var arr = [];
    for (let i = 0; i < values.length; i++) {
      var recurring = true;
      var days_count = 0;
      var item_text = '';
      for (let ii = 0; ii < values[i].weekdays.length; ii++) {
        days_count += 1;
        var day = values[i].weekdays[ii];
        if (day === 0) {
          item_text += 'Everyday, ';
        }
        if (day === 1) {
          item_text += 'Sun, ';
        }
        if (day === 2) {
          item_text += 'Mon, ';
        }
        if (day === 3) {
          item_text += 'Tue, ';
        }
        if (day === 4) {
          item_text += 'Wed, ';
        }
        if (day === 5) {
          item_text += 'Thu, ';
        }
        if (day === 6) {
          item_text += 'Fri, ';
        }
        if (day === 7) {
          item_text += 'Sat, ';
        }
      }
      if (days_count === 0) {
        recurring = false;
        var date = new Date(values[i].timestamp * 1000);
        var year = date.getFullYear();
        var month = date.getMonth();
        var day = date.getDate();
        var hours = date.getHours();
        var minutes = '0' + date.getMinutes();
        var seconds = '0' + date.getSeconds();
        var formattedTime =
          year +
          '-' +
          ('0' + (month + 1)).slice(-2) +
          '-' +
          ('0' + day).slice(-2) +
          ', ' +
          ('0' + hours).slice(-2) +
          ':' +
          minutes.substr(-2);
        item_text += formattedTime;
        //if (value[i].timestamp < time.now) {
        //  continue;
        //}
        arr.push({
          text: item_text,
          recurring: recurring,
          timestamp: values[i].timestamp,
        });
      } else {
        const zeroPad = (num, places) => String(num).padStart(places, '0');
        var hour = zeroPad(values[i].hour, 2);
        var minute = zeroPad(values[i].minute, 2);
        item_text += hour + ':' + minute;
        arr.push({
          text: item_text,
          recurring: recurring,
          weekdays: values[i].weekdays,
          hour: values[i].hour,
          minute: values[i].minute,
        });
      }
      //console.log(item_text, recurring);
    }
    setTimetable([...arr]);
  }

  function onConnected(msg) {
    console.log('Connected!!!!');
    setConnected(true);
  }

  function onConnectionLost() {
    setConnected(false);
    console.log('lost Connection!');
  }

  var reconnect = false;

  function connect() {
    if (connected === true) {
      console.log('disconnect!', connected);
      client.disconnect();
      setTimeout(connectClient, 1000);
    } else {
      connectClient();
    }
  }

  async function connectClient() {
    if (reconnect === true) {
      console.log('disconnect!', connected);
      client.disconnect();
      setTimeout(connect, 2000);
      reconnect = false;
    }
    client.connect({
      useSSL: true,
      userName: name,
      password: password,
      keepAliveInterval: 2,
      cleanSession: true,
      onSuccess: () => {
        console.log('Connected!');
        client.subscribe(`katzomat/${name}/status`);
        client.subscribe(`katzomat/${name}/status/verbose`);
        client.subscribe(`katzomat/${name}/images/#`);
        client.subscribe(`katzomat/${name}/feed`);
        client.subscribe(`katzomat/${name}/timetable`);
        client.subscribe(`katzomat/${name}/settings`);
        client.onMessageArrived = onMessage;
        client.onConnected = onConnected;
        client.onConnectionLost = onConnectionLost;
      },
      onFailure: () => {
        console.log('Failed to connect!');
      },
    });
  }

  const alert = (flag, data) => {
    if (connected === false) {
      connectionAlert();
      return;
    }
    if (
      status === 'Katzomat Status: unkown' ||
      status === 'Katzomat Status: offline'
    ) {
      offlineAlert(flag, data);
      return;
    } else {
      command(flag, data);
    }
  };

  const offlineAlert = (flag, data) =>
    Alert.alert(
      'Katzomat Offline',
      'The Katzomat will the command as soon it goes online.',
      [
        {
          text: 'Cancel',
          onPress: () => console.log('Cancel Pressed'),
          style: 'cancel',
        },
        {text: 'Send it anyway', onPress: () => command(flag, data)},
      ],
    );

  const command = (flag, data) => {
    console.log(flag);
    switch (flag) {
      case 'refresh':
        refresh();
        break;
      case 'feed':
        feedAlert();
        break;
      case 'sendTimetable':
        sendTimetable(data);
        break;
      case 'sendDeviceSettings':
        saveDeviceSettings(rotationTime);
        sendDeviceSettings(rotationTime);
        break;
    }
  };

  const feedAlert = () =>
    Alert.alert('Feed The Cat', 'Do you want to feed the cat?', [
      {
        text: 'Cancel',
        onPress: () => console.log('Cancel Pressed'),
        style: 'cancel',
      },
      {text: 'OK', onPress: () => feed()},
    ]);

  const connectionAlert = () =>
    Alert.alert('Not connected!', 'Please connect to the KatzoMQTT.', [
      {text: 'Settings', onPress: () => console.log('Go to Settings')},
      {
        text: 'Cancel',
        onPress: () => console.log('Cancel Pressed'),
        style: 'cancel',
      },
      {text: 'Connect', onPress: () => connect()},
    ]);

  function sendTimetable(data) {
    var timestamp = Date.now() / 1000;
    timestamp = parseInt(timestamp, 10);
    var payload = JSON.stringify({timestamp: timestamp, values: data});
    console.log(payload);
    const message = new Paho.Message(payload);
    message.destinationName = `katzomat/${name}/timetable`;
    message.qos = 2;
    message.retained = true;
    client.send(message);
    console.log('sent timetable');
  }

  function feed() {
    const message = new Paho.Message('0');
    message.destinationName = `katzomat/${name}/feed`;
    message.qos = 2;
    message.retained = true;
    client.send(message);
    console.log('sent');
  }

  function refresh() {
    const message = new Paho.Message('0');
    message.destinationName = `katzomat/${name}/images/take`;
    message.qos = 2;
    message.retained = true;
    client.send(message);
    console.log('sent');
  }

  async function saveAccountSettings(settings) {
    console.log(settings);
    try {
      await EncryptedStorage.setItem(
        'accountSettings',
        JSON.stringify({
          settings,
        }),
      );
      connect();
      console.log('saveAccountSettings');
      // Congrats! You've just stored your first value!
    } catch (error) {
      // There was an error on the native side
      console.log(error);
    }
  }

  async function loadAccountSettings() {
    try {
      const session = await EncryptedStorage.getItem('accountSettings');

      if (session !== undefined) {
        const obj = JSON.parse(session);
        setDomain(obj.settings.domain);
        setName(obj.settings.name);
        setPassword(obj.settings.password);
      }
    } catch (error) {
      console.log(error);
      // There was an error on the native side
    }
  }

  function saveAndSendDeviceSettings(rotationTime) {
    console.log('saveDeviceSettings');

    if (connected === false) {
      connectionAlert();
      return;
    } else {
      const message = new Paho.Message(rotationTime);
      message.destinationName = `katzomat/${name}/settings`;
      message.qos = 2;
      message.retained = true;
      client.send(message);
    }

    saveDeviceSettings(rotationTime);
  }

  function sendDeviceSettings(rotationTime) {
    console.log(rotationTime);
    const message = new Paho.Message(rotationTime);
    message.destinationName = `katzomat/${name}/settings`;
    message.qos = 2;
    message.retained = true;
    client.send(message);
    console.log(message);
  }

  async function saveDeviceSettings(rotationTime) {
    try {
      await EncryptedStorage.setItem(
        'deviceSettings',
        JSON.stringify({
          rotationTime,
        }),
      );
      console.log('savedDeviceSettings');
    } catch (error) {
      console.log(error);
    }
  }

  async function loadDeviceSettings() {
    try {
      const session = await EncryptedStorage.getItem('deviceSettings');

      if (session !== undefined) {
        const obj = JSON.parse(session);
        setRotationTime(obj.rotationTime);
      }
    } catch (error) {
      console.log(error);
      // There was an error on the native side
    }
  }

  React.useEffect(() => {
    loadAccountSettings();
    loadDeviceSettings();
  }, []);

  return (
    <SafeAreaProvider>
      <NavigationContainer>
        {/*<NativeStackExample /> */}
        <Tab.Navigator
          screenOptions={{
            tabBarActiveTintColor: 'tomato',
            tabBarInactiveTintColor: 'blue',
            headerTintColor: 'white',
            headerTitleStyle: {
              fontWeight: 'bold',
            },
            headerStyle: {
              backgroundColor: 'red',
            },
          }}>
          <Tab.Screen
            name="HomeTab"
            //component={HomeTabScreen}
            options={{
              tabBarBadge: 3,
              title: 'Home',
              headerTitle: `${status}`,
              tabBarIcon: ({size, color}) => <Icon name={'home'} size={20} />,
            }}>
            {props => (
              <HomeTabScreen
                {...props}
                connect={connect}
                feed={feed}
                refresh={refresh}
                timetable={timetable}
                alert={alert}
              />
            )}
          </Tab.Screen>
          <Tab.Screen
            name="Timetable"
            //component={HomeTabScreen}
            options={{
              tabBarBadge: 3,
              title: 'Timetable',
              headerTitle: `${status}`,
              tabBarIcon: ({size, color}) => <Icon name={'home'} size={20} />,
            }}>
            {props => (
              <TimeTabScreen
                {...props}
                connect={connect}
                command={command}
                timetable={timetable}
                alert={alert}
              />
            )}
          </Tab.Screen>
          <Tab.Screen
            name="SettingsTab"
            //component={SettingsTabScreen}
            options={{
              title: 'Settings',
              headerTitle: `${status}`,
              tabBarIcon: ({size, color}) => (
                <Icon name={'setting'} size={20} />
              ),
            }}>
            {props => (
              <SettingsTabScreen
                {...props}
                connect={connect}
                saveAccountSettings={saveAccountSettings}
                loadAccountSettings={loadAccountSettings}
                timetable={timetable}
                alert={alert}
              />
            )}
          </Tab.Screen>
        </Tab.Navigator>
      </NavigationContainer>
    </SafeAreaProvider>
  );
};

interface ChildProps {
  parentFunction: Function;
}

function HomeTabScreen(route) {
  //const {connected, setConnected} = React.useContext(PageContext);
  const {connected, setConnected, imgs, setImgs} =
    React.useContext(PageContext);
  const [imgActive, setimgActive] = useState(0);
  const [value, setValue] = useState([]);

  React.useLayoutEffect(() => {
    route.navigation.setOptions({
      //headerTitle: `Count is ${status}`, //use this to make the title changeable
      headerRight: () => (
        <View style={styles.connectButton}>
          <Button
            title={connected ? 'Connected' : 'Log In'}
            color="black"
            onPress={() => route.connect()}
          />
        </View>
      ),
    });
  }, [route, route.navigation, connected]); // ← This `connected` here ensures that the header state is updated

  useEffect(() => {
    console.log('value changed');
  }, [imgs]);

  const onchange = nativeEvent => {
    if (nativeEvent) {
      const slide = Math.ceil(
        nativeEvent.contentOffset.x / nativeEvent.layoutMeasurement.width,
      );
      //console.log("slide");
      //console.log(slide);
      //console.log(imgActive);
      if (slide !== imgActive) {
        if (slide >= imgs.length) {
          return;
        }
        setimgActive(slide);
      }
    }
  };

  function scrollToEndOnNewImg(arrLength) {
    if (this._scrollView) {
      if (imgActive !== arrLength) {
        //this._scrollView.scrollToEnd({animated: true});
      }
    }
  }

  function add() {
    //ar x = [...value, `Entry ${value.length}`];
    var y = {
      timestamp: 1661360552, //1661360551
      uri: 'https://upload.wikimedia.org/wikipedia/commons/thumb/5/5d/Eclectus_roratus_in_Zoo_Augsburg.jpg/1280px-Eclectus_roratus_in_Zoo_Augsburg.jpg',
      date: 'fsfasdf',
    };
    //x.push(2);
    setValue([...value, y]);
    console.log('whut');
    imgs.map(e => {
      console.log(e);
    });
    console.log(imgs);

    //arr.sort((a, b) => a.timestamp - b.timestamp);
    //console.log(base64Img);
    //console.log(imgs);
    //console.log(arr);
    //console.log(value);
    setImgs([...y, y]);
  }

  return (
    <View style={styles.page}>
      <SafeAreaView style={styles.container}>
        <View style={styles.wrap}>
          <Button title="add" onPress={() => add()} />
          <ScrollView
            ref={view => (this._scrollView = view)}
            onScroll={({nativeEvent}) => onchange(nativeEvent)}
            //onScroll={onChange}
            showsHorizontalScrollIndicator={false}
            pagingEnabeled
            horizontal
            style={styles.wrap}>
            {imgs.map((e, index) => {
              return (
                <Image
                  key={index}
                  resizeMode="contain"
                  style={styles.wrapImg}
                  source={{uri: e.uri}}
                />
              );
            }, scrollToEndOnNewImg(imgs.length))}
          </ScrollView>
        </View>
        <View style={styles.wrapDot}>
          {imgs.map((e, index) => {
            return (
              <Text
                key={index}
                style={imgActive === index ? styles.dotActive : styles.dot}>
                ●
              </Text>
            );
          })}
        </View>
        <View style={styles.wrapDate}>
          <Text>{imgs[imgActive].date}</Text>
        </View>
        <View style={styles.button}>
          <Button title={'Feed The Cat'} onPress={() => route.alert('feed')} />
        </View>
        <View style={styles.button}>
          <Button
            title={'Refresh Image'}
            onPress={() => route.alert('refresh')}
          />
        </View>
      </SafeAreaView>
    </View>
  );
}

function TimeTabScreen(route) {
  const {connected, setConnected, status, timetable, setTimetable} =
    React.useContext(PageContext);
  //const [connected, setConnected] = React.useContext(PageContext);
  //const [timetable, setTimetable] = useState([]);
  const [modalWeekdaysVisible, setModalWeekdaysVisible] = useState(false);

  const [weekdays, setWeekdays] = useState([]);
  const options = [
    'Monday',
    'Tuesday',
    'Wednesday',
    'Thursday',
    'Friday',
    'Saturday',
    'Sunday',
  ];

  //const [date, setDate] = useState(new Date(0));
  //const [time, setTime] = useState(new Date(0));
  var showTimePickerAfter = false;
  var mode = '';
  var recurring = true;
  var date = new Date(Date.now());
  var time = new Date(Date.now());

  const onDateChange = (event, selectedDate) => {
    console.log(mode);
    console.log(selectedDate);
    const currentDate = selectedDate;
    if (mode === 'date') {
      //setDate(currentDate);
      date = currentDate;
    }
    if (mode === 'time') {
      //setTime(currentDate);
      time = currentDate;
      console.log('?', recurring);
      if (recurring === false) {
        console.log('date...');
        var arr = timetable;
        var year = date.getFullYear();
        var month = date.getMonth();
        var day = String(date.getDate()).padStart(2, '0');
        var hours = time.getHours();
        var minutes = '0' + time.getMinutes();
        var formattedTime =
          year +
          '-' +
          ('0' + (month + 1)).slice(-2) +
          '-' +
          day +
          ', ' +
          ('0' + hours).slice(-2) +
          ':' +
          minutes.substr(-2);

        var tempDate = new Date(
          `${year}-${month + 1}-${day}T${hours}:${minutes}:00`,
        );
        var timestamp = tempDate.getTime() / 1000;
        arr.push({
          text: formattedTime,
          recurring: false,
          weekdays: [],
          timestamp: timestamp,
        });
        console.log(formattedTime);
        setTimetable([...arr]);
        route.command('sendTimetable', timetable);
      }
    }
    if (showTimePickerAfter) {
      showTimePickerAfter = false;
      showTimepicker();
    }
  };

  const showMode = currentMode => {
    DateTimePickerAndroid.open({
      value: date,
      onChange: onDateChange,
      mode: currentMode,
      is24Hour: true,
    });
  };

  const showDatepicker = boolean => {
    showTimePickerAfter = boolean;
    mode = 'date';
    showMode(mode);
  };

  const showTimepicker = () => {
    mode = 'time';
    showMode(mode);
  };

  React.useLayoutEffect(() => {
    route.navigation.setOptions({
      //headerTitle: `Count is ${status}`, //use this to make the title changeable
      headerRight: () => (
        <View style={styles.connectButton}>
          <Button
            title={connected ? 'Connected' : 'Log In'}
            color="black"
            onPress={() => route.connect()}
          />
        </View>
      ),
    });
  }, [route, route.navigation, connected]); // ← This `connected` here ensures that the header state is updated

  function removeTimetableEntry(index) {
    var arr = timetable;
    arr.splice(index, 1);
    if (arr.length === 0) {
      setTimetable([]);
      route.alert('sendTimetable', timetable);
      return;
    }
    console.log('arr2', index, arr);
    setTimetable([...arr]);
    route.alert('sendTimetable', timetable);
  }

  function pickWeekday(selectedWeekday) {
    //const index = weekdays.findIndex(weekday => weekday === selectedWeekday);

    if (weekdays.includes(selectedWeekday)) {
      setWeekdays(weekdays.filter(weekday => weekday !== selectedWeekday));
      return;
    }

    setWeekdays(weekday => weekdays.concat(selectedWeekday));
    //console.log(weekdays);
  }

  const connectionAlert = () =>
    Alert.alert('Not connected!', 'Please connect to the KatzoMQTT.', [
      {text: 'Settings', onPress: () => console.log('Go to Settings')},
      {
        text: 'Cancel',
        onPress: () => console.log('Cancel Pressed'),
        style: 'cancel',
      },
      {text: 'Connect', onPress: () => route.connect()},
    ]);

  const alert = (flag, data) => {
    if (connected === false) {
      connectionAlert();
      return;
    }
    if (
      status === 'Katzomat Status: unkown' ||
      status === 'Katzomat Status: offline'
    ) {
      offlineAlert(flag, data);
      return;
    } else {
      command(flag, data);
    }
  };

  const offlineAlert = (flag, data) =>
    Alert.alert(
      'Katzomat Offline',
      'The Katzomat will the command as soon it goes online.',
      [
        {
          text: 'Cancel',
          onPress: () => console.log('Cancel Pressed'),
          style: 'cancel',
        },
        {text: 'Send it anyway', onPress: () => command(flag, data)},
      ],
    );

  const command = (flag, data) => {
    console.log(flag);
    switch (flag) {
      case 'timetableAlert':
        timetableAlert();
        break;
    }
  };

  const timetableAlert = () => {
    Alert.alert(
      'Recurring?',
      'Do you want to add a timetable entry that is recurring or just onetime?',
      [
        {
          text: 'Cancel',
          onPress: () => console.log('Cancel Pressed'),
          style: 'cancel',
        },
        {
          text: 'Recurring',
          onPress: () => {
            setModalWeekdaysVisible(!modalWeekdaysVisible);
            recurring = true;
          },
        },
        {
          text: 'Onetime Only',
          onPress: () => {
            showDatepicker(true);
            recurring = false;
          },
        },
      ],
    );
  };

  return (
    <View style={styles.page}>
      <SafeAreaView style={styles.container}>
        <View style={styles.wrap}>
          <ScrollView
            ref={view => (this._scrollView = view)}
            //onScroll={({nativeEvent}) => onchange(nativeEvent)}
            //onScroll={onChange}
            //showsHorizontalScrollIndicator={false}
            pagingEnabeled
            vertical
            style={styles.wrap}>
            {timetable.map((e, index) => {
              console.log(index, 'whut', e);
              return (
                <View key={index}>
                  <Text>{e.text}</Text>
                  <Button
                    title={'Remove Entry'}
                    onPress={() => removeTimetableEntry(index)}
                  />
                </View>
              );
            })}
          </ScrollView>
          <Modal
            animationType="slide"
            transparent={true}
            visible={modalWeekdaysVisible}
            onRequestClose={() => {
              setModalWeekdaysVisible(!modalWeekdaysVisible);
            }}>
            <View style={styles.centeredView}>
              <View style={styles.modalView}>
                <View style={styles.options}>
                  <Text>Pick your feeding days and time:</Text>
                  {options.map(option => (
                    <View style={{flexDirection: 'row'}} key={option}>
                      <Pressable
                        style={styles.checkBox}
                        onPress={() => pickWeekday(option)}>
                        {weekdays.includes(option) && (
                          <Text style={styles.check}>✓</Text>
                        )}
                      </Pressable>
                      <Text style={styles.weekdayName}>{option}</Text>
                    </View>
                  ))}
                </View>

                <Pressable
                  style={[styles.button, styles.buttonClose]}
                  onPress={() => showTimepicker()}>
                  <Text>{time.toLocaleTimeString()}</Text>
                </Pressable>

                <Pressable
                  style={[styles.button, styles.buttonClose]}
                  onPress={() =>
                    setModalWeekdaysVisible(!modalWeekdaysVisible)
                  }>
                  <Text style={styles.textStyle}>Cancel</Text>
                </Pressable>
                <Pressable
                  style={[styles.button, styles.buttonClose]}
                  onPress={() => {
                    setModalWeekdaysVisible(!modalWeekdaysVisible);
                    var arr = [];
                    arr = timetable;
                    var hours = time.getHours();
                    var minutes = '0' + time.getMinutes();
                    var weeksdaysStr = '';
                    var weekdaysArr = [];
                    if (weekdays.length === 0) {
                      weekdaysArr = [0];
                    }
                    weekdays.map(day => {
                      weeksdaysStr += day;
                      weeksdaysStr += ', ';
                      if (day === 'Everyday') {
                        weekdaysArr.push(0);
                      }
                      if (day === 'Sunday') {
                        weekdaysArr.push(1);
                      }
                      if (day === 'Monday') {
                        weekdaysArr.push(2);
                      }
                      if (day === 'Tuesday') {
                        weekdaysArr.push(3);
                      }
                      if (day === 'Wednesday') {
                        weekdaysArr.push(4);
                      }
                      if (day === 'Thursday') {
                        weekdaysArr.push(5);
                      }
                      if (day === 'Friday') {
                        weekdaysArr.push(6);
                      }
                      if (day === 'Saturday') {
                        weekdaysArr.push(7);
                      }
                    });
                    if (weekdays.length === 0 || weekdays.length === 7) {
                      weeksdaysStr = 'Everyday, ';
                    }
                    var formattedTime =
                      ('0' + hours).slice(-2) + ':' + minutes.substr(-2);
                    arr.push({
                      text: `${weeksdaysStr}${formattedTime}`,
                      recurring: true,
                      weekdays: weekdaysArr,
                      hour: hours,
                      minute: time.getMinutes(),
                    });
                    setTimetable([...arr]);
                    route.command('sendTimetable', timetable);
                  }}>
                  <Text style={styles.textStyle}>Ok</Text>
                </Pressable>
              </View>
            </View>
          </Modal>
          <View>
            <Button
              title={'Add Timetable'}
              //onPress={() => route.alert('addTimetable')}
              onPress={() => alert('timetableAlert')}
            />
            <Text>selected: {date.toDateString()}</Text>
            <Text>selected: {time.toLocaleTimeString()}</Text>
          </View>
        </View>
      </SafeAreaView>
    </View>
  );
}

function SettingsTabScreen(route, navigation) {
  const {
    connected,
    domain,
    setDomain,
    name,
    setName,
    password,
    setPassword,
    rotationTime,
    setRotationTime,
  } = React.useContext(PageContext);
  const [modalLogVisible, setModalLogVisible] = useState(false);
  var log = {
    time: 1661362594,
    history: [
      1655735536, 1655735520, 1655731133, 1655731121, 1655730389, 1655730048,
      1655729820, 1655728624, 1655728006, 1655726862, 1655726756, 1655726656,
      1655726623, 0,
    ],
  };
  //console.log(status);
  React.useEffect(() => {
    route.navigation.setOptions({
      headerRight: () => (
        <View style={styles.connectButton}>
          <Button
            title={connected ? 'Connected' : 'Log In'}
            color="black"
            onPress={() => route.connect()}
          />
        </View>
      ),
    });
  }, [route, route.navigation, connected]);

  function onTextChanged(value) {
    // code to remove non-numeric characters from text
    //this.setState({number: value.replace(/[- #*;,.<>\{\}\[\]\\\/]/gi, '')});
    setRotationTime(value.replace(/[^0-9]/g, ''));
  }

  return (
    <View style={styles.page}>
      <Text>Settings Tab! </Text>
      <Text>Account Settings: </Text>
      <TextInput
        style={styles.input}
        maxLength={20}
        onChangeText={setDomain}
        value={domain}
        placeholder="Katzapp Domain"
        keyboardType="url"
      />
      <TextInput
        style={styles.input}
        maxLength={20}
        onChangeText={setName}
        value={name}
        placeholder="Account Name"
        keyboardType="default"
      />
      <TextInput
        style={styles.input}
        maxLength={20}
        onChangeText={setPassword}
        value={password}
        placeholder="Account password"
        keyboardType="default"
      />
      <Button
        title="Save"
        color="black"
        onPress={() =>
          route.saveAccountSettings({
            domain: domain,
            name: name,
            password: password,
          })
        }
      />
      <Text>Device Settings: </Text>
      <Text>Rotation Time in Milliseconds: </Text>
      <TextInput
        style={styles.input}
        maxLength={10}
        //onChangeText={setRotationTime}
        onChangeText={value => onTextChanged(value)}
        value={rotationTime}
        placeholder="Rotation Time here"
        keyboardType="default"
      />
      <Button
        title="Save"
        color="black"
        onPress={() => route.alert('sendDeviceSettings')}
      />
      <View style={styles.button}>
        <Button
          title="Log"
          color="black"
          onPress={() => setModalLogVisible(!modalLogVisible)}
        />
      </View>
      <Modal
        animationType="slide"
        transparent={true}
        visible={modalLogVisible}
        onRequestClose={() => {
          setModalLogVisible(!modalLogVisible);
        }}>
        <View style={styles.centeredView}>
          <View style={styles.modalView}>
            <View style={styles.centeredView}>
              <Text>Katzomat time: {log.time}</Text>
              <Text>The Cat was fed on the following dates:</Text>
              {log.history.map(l => (
                <View key={l}>
                  <Text>{l}</Text>
                </View>
              ))}
              <Button
                title="Close"
                color="black"
                onPress={() => setModalLogVisible(!modalLogVisible)}
              />
            </View>
          </View>
        </View>
      </Modal>
    </View>
  );
}

const styles = StyleSheet.create({
  check: {
    alignSelf: 'center',
  },
  weekdayName: {
    textTransform: 'capitalize',
    fontSize: 16,
  },
  checkBox: {
    width: 25,
    height: 25,
    borderWidth: 2,
    borderColor: 'green',
    marginRight: 5,
  },
  weekdays: {
    flexDirection: 'row',
  },
  options: {
    alignSelf: 'flex-start',
    marginLeft: 50,
  },
  container: {
    flex: 1,
  },
  wrap: {
    width: WIDTH,
    height: HEIGHT * 0.5,
  },
  wrapImg: {
    width: WIDTH,
    height: HEIGHT * 0.5,
  },
  wrapDot: {
    flexDirection: 'row',
    alignSelf: 'center',
  },
  dotActive: {
    margin: 3,
    color: 'black',
  },
  dot: {
    margin: 3,
    color: 'red',
  },
  wrapDate: {
    alignSelf: 'center',
  },
  centeredView: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    marginTop: 22,
  },
  modalView: {
    margin: 20,
    backgroundColor: 'blue',
    borderRadius: 20,
    padding: 50,
    alignItems: 'center',
    shadowColor: '#000',
    shadowOffset: {
      width: 0,
      height: 2,
    },
    shadowOpacity: 0.25,
    shadowRadius: 4,
    elevation: 5,
  },
  page: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#6ED4C8',
    padding: 30,
  },
  header: {
    headerTintColor: 'black',
    headerTitleStyle: {
      fontWeight: 'bold',
    },
    headerStyle: {
      backgroundColor: 'red',
      paddingHorizontal: 15,
    },
  },
  connectButton: {
    margin: 5,
  },
  button: {
    margin: 10,
    //padding: 20,
  },
  input: {
    alignSelf: 'stretch',
    flexDirection: 'row',
    height: 30,
    margin: 6,
    borderWidth: 1,
    padding: 5,
  },
});
